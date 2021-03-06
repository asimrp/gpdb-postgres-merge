/*-------------------------------------------------------------------------
 *
 * defrem.h
 *	  POSTGRES define and remove utility definitions.
 *
 *
 * Portions Copyright (c) 1996-2009, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $PostgreSQL: pgsql/src/include/commands/defrem.h,v 1.98 2009/12/07 05:22:23 tgl Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef DEFREM_H
#define DEFREM_H

#include "nodes/parsenodes.h"
#include "utils/array.h"

struct HTAB;  /* utils/hsearch.h */

/* commands/indexcmds.c */
extern void DefineIndex(RangeVar *heapRelation,
			char *indexRelationName,
			Oid indexRelationId,
			char *accessMethodName,
			char *tableSpaceName,
			List *attributeList,
			Expr *predicate,
			List *options,
			List *exclusionOpNames,
			bool unique,
			bool primary,
			bool isconstraint,
			bool deferrable,
			bool initdeferred,
			bool is_alter_table,
			bool check_rights,
			bool skip_build,
			bool quiet,
			bool concurrent,
			IndexStmt *stmt /* MPP */);
extern void ReindexIndex(ReindexStmt *stmt);
extern void ReindexTable(ReindexStmt *stmt);
extern void ReindexDatabase(ReindexStmt *stmt);
extern char *makeObjectName(const char *name1, const char *name2,
			   const char *label);
extern char *ChooseRelationName(const char *name1, const char *name2,
				   const char *label, Oid namespaceid);
extern char *ChooseRelationNameWithCache(const char *name1, const char *name2,
								const char *label, Oid namespaceid, 
								struct HTAB *cache);
extern Oid	GetDefaultOpClass(Oid type_id, Oid am_id);

/* commands/functioncmds.c */
extern void CreateFunction(CreateFunctionStmt *stmt, const char *queryString);
extern void RemoveFunction(RemoveFuncStmt *stmt);
extern void RemoveFunctionById(Oid funcOid);
extern void SetFunctionReturnType(Oid funcOid, Oid newRetType);
extern void SetFunctionArgType(Oid funcOid, int argIndex, Oid newArgType);
extern void RenameFunction(List *name, List *argtypes, const char *newname);
extern void AlterFunctionOwner(List *name, List *argtypes, Oid newOwnerId);
extern void AlterFunctionOwner_oid(Oid procOid, Oid newOwnerId);
extern void AlterFunction(AlterFunctionStmt *stmt);
extern void CreateCast(CreateCastStmt *stmt);
extern void DropCast(DropCastStmt *stmt);
extern void DropCastById(Oid castOid);
extern Oid  get_cast_oid(Oid sourcetypeid, Oid targettypeid, bool missing_ok);
extern void interpret_function_parameter_list(List *parameters,
								  Oid languageOid,
								  bool is_aggregate,
								  const char *queryString,
								  oidvector **parameterTypes,
								  ArrayType **allParameterTypes,
								  ArrayType **parameterModes,
								  ArrayType **parameterNames,
								  List **parameterDefaults,
								  Oid *variadicArgType,
								  Oid *requiredResultType);
extern void AlterFunctionNamespace(List *name, List *argtypes, bool isagg,
					   const char *newschema);
extern Oid	AlterFunctionNamespace_oid(Oid procOid, Oid nspOid);
extern void ExecuteDoStmt(DoStmt *stmt);

/* commands/operatorcmds.c */
extern void DefineOperator(List *names, List *parameters);
extern void RemoveOperator(RemoveFuncStmt *stmt);
extern void RemoveOperatorById(Oid operOid);
extern void AlterOperatorOwner(List *name, TypeName *typeName1,
				   TypeName *typename2, Oid newOwnerId);
extern void AlterOperatorOwner_oid(Oid operOid, Oid newOwnerId);
extern void AlterOperatorNamespace(List *names, List *argtypes, const char *newschema);
extern Oid	AlterOperatorNamespace_oid(Oid operOid, Oid newNspOid);

/* commands/aggregatecmds.c */
extern void DefineAggregate(List *name, List *args, bool oldstyle,
							List *parameters, bool ordered, const char *queryString);
extern void RemoveAggregate(RemoveFuncStmt *stmt);
extern void RenameAggregate(List *name, List *args, const char *newname);
extern void AlterAggregateOwner(List *name, List *args, Oid newOwnerId);

/* commands/opclasscmds.c */
extern void DefineOpClass(CreateOpClassStmt *stmt);
extern void DefineOpFamily(CreateOpFamilyStmt *stmt);
extern void AlterOpFamily(AlterOpFamilyStmt *stmt);
extern void RemoveOpClass(RemoveOpClassStmt *stmt);
extern void RemoveOpFamily(RemoveOpFamilyStmt *stmt);
extern void RemoveOpClassById(Oid opclassOid);
extern void RemoveOpFamilyById(Oid opfamilyOid);
extern void RemoveAmOpEntryById(Oid entryOid);
extern void RemoveAmProcEntryById(Oid entryOid);
extern void RenameOpClass(List *name, const char *access_method, const char *newname);
extern void RenameOpFamily(List *name, const char *access_method, const char *newname);
extern void AlterOpClassOwner(List *name, const char *access_method, Oid newOwnerId);
extern void AlterOpClassOwner_oid(Oid opclassOid, Oid newOwnerId);
extern void AlterOpClassNamespace(List *name, char *access_method, const char *newschema);
extern Oid	AlterOpClassNamespace_oid(Oid opclassOid, Oid newNspOid);
extern void AlterOpFamilyOwner(List *name, const char *access_method, Oid newOwnerId);
extern void AlterOpFamilyOwner_oid(Oid opfamilyOid, Oid newOwnerId);
extern void AlterOpFamilyNamespace(List *name, char *access_method, const char *newschema);
extern Oid	AlterOpFamilyNamespace_oid(Oid opfamilyOid, Oid newNspOid);
extern Oid  get_am_oid(const char *amname, bool missing_ok);
extern Oid  get_opclass_oid(Oid amID, List *opclassname, bool missing_ok);
extern Oid  get_opfamily_oid(Oid amID, List *opfamilyname, bool missing_ok);

/* commands/tsearchcmds.c */
extern void DefineTSParser(List *names, List *parameters);
extern void RenameTSParser(List *oldname, const char *newname);
extern void RemoveTSParsers(DropStmt *drop);
extern void RemoveTSParserById(Oid prsId);

extern void DefineTSDictionary(List *names, List *parameters);
extern void RenameTSDictionary(List *oldname, const char *newname);
extern void RemoveTSDictionaries(DropStmt *drop);
extern void RemoveTSDictionaryById(Oid dictId);
extern void AlterTSDictionary(AlterTSDictionaryStmt *stmt);
extern void AlterTSDictionaryOwner(List *name, Oid newOwnerId);

extern void DefineTSTemplate(List *names, List *parameters);
extern void RenameTSTemplate(List *oldname, const char *newname);
extern void RemoveTSTemplates(DropStmt *stmt);
extern void RemoveTSTemplateById(Oid tmplId);

extern void DefineTSConfiguration(List *names, List *parameters);
extern void RenameTSConfiguration(List *oldname, const char *newname);
extern void RemoveTSConfigurations(DropStmt *stmt);
extern void RemoveTSConfigurationById(Oid cfgId);
extern void AlterTSConfiguration(AlterTSConfigurationStmt *stmt);
extern void AlterTSConfigurationOwner(List *name, Oid newOwnerId);

extern text *serialize_deflist(List *deflist);
extern List *deserialize_deflist(Datum txt);

/* commands/foreigncmds.c */
extern void AlterForeignServerOwner(const char *name, Oid newOwnerId);
extern void AlterForeignDataWrapperOwner(const char *name, Oid newOwnerId);
extern void CreateForeignDataWrapper(CreateFdwStmt *stmt);
extern void AlterForeignDataWrapper(AlterFdwStmt *stmt);
extern void RemoveForeignDataWrapper(DropFdwStmt *stmt);
extern void RemoveForeignDataWrapperById(Oid fdwId);
extern void CreateForeignServer(CreateForeignServerStmt *stmt);
extern void AlterForeignServer(AlterForeignServerStmt *stmt);
extern void RemoveForeignServer(DropForeignServerStmt *stmt);
extern void RemoveForeignServerById(Oid srvId);
extern void CreateUserMapping(CreateUserMappingStmt *stmt);
extern void AlterUserMapping(AlterUserMappingStmt *stmt);
extern void RemoveUserMapping(DropUserMappingStmt *stmt);
extern void RemoveUserMappingById(Oid umId);

/* support routines in commands/define.c */

extern char *case_translate_language_name(const char *input);

extern char *defGetString(DefElem *def);
extern double defGetNumeric(DefElem *def);
extern bool defGetBoolean(DefElem *def);
extern int64 defGetInt64(DefElem *def);
extern List *defGetQualifiedName(DefElem *def);
extern TypeName *defGetTypeName(DefElem *def);
extern int	defGetTypeLength(DefElem *def);
extern DefElem *defWithOids(bool value);

#endif   /* DEFREM_H */
