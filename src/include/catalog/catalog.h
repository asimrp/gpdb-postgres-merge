/*-------------------------------------------------------------------------
 *
 * catalog.h
 *	  prototypes for functions in backend/catalog/catalog.c
 *
 *
 * Portions Copyright (c) 1996-2009, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $PostgreSQL: pgsql/src/include/catalog/catalog.h,v 1.44 2009/06/11 14:49:09 momjian Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef CATALOG_H
#define CATALOG_H

#include "catalog/catversion.h"
#include "catalog/pg_class.h"
#include "storage/relfilenode.h"
#include "utils/relcache.h"

#include "catalog/oid_dispatch.h"

#define OIDCHARS		10		/* max chars printed by %u */
/*
 * In PostgreSQL, this is called just TABLESPACE_VERSION_DIRECTORY. But in 
 * GPDB, you should use tablespace_version_directory() function instead.
 * This constant has been renamed so that we catch and know to modify all
 * upstream uses of TABLESPACE_VERSION_DIRECTORY.
 */
#define GP_TABLESPACE_VERSION_DIRECTORY	"GPDB_" GP_MAJORVERSION "_" \
									CppAsString2(CATALOG_VERSION_NO)

extern const char *forkNames[];
extern ForkNumber forkname_to_number(char *forkName);

extern char *relpath(RelFileNode rnode, ForkNumber forknum);
extern char *GetDatabasePath(Oid dbNode, Oid spcNode);

extern void reldir_and_filename(RelFileNode rnode, ForkNumber forknum,
					char **dir, char **filename);

extern bool IsSystemRelation(Relation relation);
extern bool IsToastRelation(Relation relation);

extern bool IsSystemClass(Form_pg_class reltuple);
extern bool IsToastClass(Form_pg_class reltuple);

extern bool IsSystemNamespace(Oid namespaceId);
extern bool IsToastNamespace(Oid namespaceId);
extern bool IsAoSegmentNamespace(Oid namespaceId);

extern bool IsReservedName(const char *name);
extern char* GetReservedPrefix(const char *name);

extern bool IsSharedRelation(Oid relationId);

extern Oid GetNewOid(Relation relation);
extern Oid GetNewOidWithIndex(Relation relation, Oid indexId,
				   AttrNumber oidcolumn);
extern Oid GetNewSequenceRelationOid(Relation relation);
extern Oid GetNewRelFileNode(Oid reltablespace, bool relisshared);

const char *tablespace_version_directory(void);

#endif   /* CATALOG_H */
