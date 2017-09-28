/*-------------------------------------------------------------------------
 *
 * lockcmds.c
 *	  Lock command support code
 *
 * Portions Copyright (c) 1996-2008, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $PostgreSQL: pgsql/src/backend/commands/lockcmds.c,v 1.18 2008/06/19 00:46:04 alvherre Exp $
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "access/heapam.h"
#include "catalog/namespace.h"
#include "commands/lockcmds.h"
#include "miscadmin.h"
#include "utils/acl.h"
#include "utils/lsyscache.h"
#include "utils/rel.h"
#include "cdb/cdbvars.h"
#include "cdb/cdbdisp_query.h"


/*
 * LOCK TABLE
 */
void
LockTableCommand(LockStmt *lockstmt)
{
	ListCell   *p;

	/*
	 * Iterate over the list and open, lock, and close the relations one at a
	 * time
	 */

	foreach(p, lockstmt->relations)
	{
		RangeVar   *relation = lfirst(p);
		Oid			reloid;
		AclResult	aclresult;
		Relation	rel;

		/*
		 * We don't want to open the relation until we've checked privilege.
		 * So, manually get the relation OID.
		 */
		reloid = RangeVarGetRelid(relation, false);

		if (lockstmt->mode == AccessShareLock)
			aclresult = pg_class_aclcheck(reloid, GetUserId(),
										  ACL_SELECT);
		else
			aclresult = pg_class_aclcheck(reloid, GetUserId(),
										  ACL_UPDATE | ACL_DELETE | ACL_TRUNCATE);

		if (aclresult != ACLCHECK_OK)
			aclcheck_error(aclresult, ACL_KIND_CLASS,
						   get_rel_name(reloid));

		if (lockstmt->nowait)
			rel = relation_open_nowait(reloid, lockstmt->mode);
		else
			rel = relation_open(reloid, lockstmt->mode);

		/* Currently, we only allow plain tables to be locked */
		if (rel->rd_rel->relkind != RELKIND_RELATION)
			ereport(ERROR,
					(errcode(ERRCODE_WRONG_OBJECT_TYPE),
					 errmsg("\"%s\" is not a table",
							relation->relname)));

		relation_close(rel, NoLock);	/* close rel, keep lock */
	}

	if (Gp_role == GP_ROLE_DISPATCH)
	{
		CdbDispatchUtilityStatement((Node *) lockstmt,
									DF_CANCEL_ON_ERROR|
									DF_WITH_SNAPSHOT|
									DF_NEED_TWO_PHASE,
									NIL,
									NULL);
	}
}
