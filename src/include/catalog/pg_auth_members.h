/*-------------------------------------------------------------------------
 *
 * pg_auth_members.h
 *	  definition of the system "authorization identifier members" relation
 *	  (pg_auth_members) along with the relation's initial contents.
 *
 *
 * Portions Copyright (c) 1996-2010, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $PostgreSQL: pgsql/src/include/catalog/pg_auth_members.h,v 1.9 2010/04/20 23:48:47 tgl Exp $
 *
 * NOTES
 *	  the genbki.pl script reads this file and generates .bki
 *	  information from the DATA() statements.
 *
 *-------------------------------------------------------------------------
 */
#ifndef PG_AUTH_MEMBERS_H
#define PG_AUTH_MEMBERS_H

#include "catalog/genbki.h"

/* ----------------
 *		pg_auth_members definition.  cpp turns this into
 *		typedef struct FormData_pg_auth_members
 * ----------------
 */
#define AuthMemRelationId	1261
#define AuthMemRelation_Rowtype_Id	2843

CATALOG(pg_auth_members,1261) BKI_SHARED_RELATION BKI_WITHOUT_OIDS BKI_ROWTYPE_OID(2843) BKI_SCHEMA_MACRO
{
	Oid			roleid;			/* ID of a role */
	Oid			member;			/* ID of a member of that role */
	Oid			grantor;		/* who granted the membership */
	bool		admin_option;	/* granted with admin option? */
} FormData_pg_auth_members;

/* GPDB added foreign key definitions for gpcheckcat. */
FOREIGN_KEY(roleid  REFERENCES pg_authid(oid));
FOREIGN_KEY(member  REFERENCES pg_authid(oid));
/*
 * NOTE: we don't mark grantor a foreign key since we don't actually
 * remove entries when a grantor is dropped. See DropRole().
 */

/* ----------------
 *		Form_pg_auth_members corresponds to a pointer to a tuple with
 *		the format of pg_auth_members relation.
 * ----------------
 */
typedef FormData_pg_auth_members *Form_pg_auth_members;

/* ----------------
 *		compiler constants for pg_auth_members
 * ----------------
 */
#define Natts_pg_auth_members				4
#define Anum_pg_auth_members_roleid			1
#define Anum_pg_auth_members_member			2
#define Anum_pg_auth_members_grantor		3
#define Anum_pg_auth_members_admin_option	4

#define Schema_pg_auth_members \
{1261, {"roleid"}      , 26, -1, 0, 4, 1, 0, -1, -1, true, 'p' ,'i', true, false, false, true, 0}, \
{1261, {"member"}      , 26, -1, 0, 4, 2, 0, -1, -1, true, 'p' ,'i', true, false, false, true, 0}, \
{1261, {"grantor"}     , 26, -1, 0, 4, 3, 0, -1, -1, true, 'p' ,'i', true, false, false, true, 0}, \
{1261, {"admin_option"}, 16, -1, 0, 1, 4, 0, -1, -1, true, 'p' ,'c', true, false, false, true, 0}

#endif   /* PG_AUTH_MEMBERS_H */
