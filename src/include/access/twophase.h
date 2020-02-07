/*-------------------------------------------------------------------------
 *
 * twophase.h
 *	  Two-phase-commit related declarations.
 *
 *
 * Portions Copyright (c) 1996-2019, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * src/include/access/twophase.h
 *
 *-------------------------------------------------------------------------
 */
#ifndef TWOPHASE_H
#define TWOPHASE_H

#include "access/xlogdefs.h"
#include "access/xact.h"
#include "datatype/timestamp.h"
#include "storage/backendid.h"

#include "cdb/cdblocaldistribxact.h"

/*
 * GlobalTransactionData is defined in twophase.c; other places have no
 * business knowing the internal definition.
 */
typedef struct GlobalTransactionData *GlobalTransaction;

/* GPDB-specific: GIDSIZE is defined in twophase.c in Postgres */

#define GIDSIZE 200

/* GPDB-specific: TwoPhaseFileHeader is defined in twophase.c in Postgres */
/*
 * Header for a 2PC state file
 */
typedef struct TwoPhaseFileHeader
{
	uint32		magic;			/* format identifier */
	uint32		total_len;		/* actual file length */
	TransactionId xid;			/* original transaction XID */
	Oid			database;		/* OID of database it was in */
	TimestampTz prepared_at;	/* time of preparation */
	Oid			owner;			/* user running the transaction */
	int32		nsubxacts;		/* number of following subxact XIDs */
	int32		ncommitrels;	/* number of delete-on-commit rels */
	int32		nabortrels;		/* number of delete-on-abort rels */
	int32		ncommitdbs;		/* number of delete-on-commit dbs */
	int32		nabortdbs;		/* number of delete-on-abort dbs */
	int32		ninvalmsgs;		/* number of cache invalidation messages */
	bool		initfileinval;	/* does relcache init file need invalidation? */
	Oid			tablespace_oid_to_delete_on_abort;
	Oid			tablespace_oid_to_delete_on_commit;
	uint16		gidlen;			/* length of the GID - GID follows the header */
	XLogRecPtr	origin_lsn;		/* lsn of this record at origin node */
	TimestampTz origin_timestamp;	/* time of prepare at origin node */
} TwoPhaseFileHeader;

/* GPDB-specific end */

/* GUC variable */
extern PGDLLIMPORT int max_prepared_xacts;

extern Size TwoPhaseShmemSize(void);
extern void TwoPhaseShmemInit(void);

extern void AtAbort_Twophase(void);
extern void PostPrepare_Twophase(void);

extern PGPROC *TwoPhaseGetDummyProc(TransactionId xid, bool lock_held);
extern BackendId TwoPhaseGetDummyBackendId(TransactionId xid, bool lock_held);

extern GlobalTransaction MarkAsPreparing(TransactionId xid,
										 LocalDistribXactData *localDistribXactRef,
										 const char *gid,
										 TimestampTz prepared_at,
										 Oid owner, Oid databaseid);

extern void StartPrepare(GlobalTransaction gxact);
extern void EndPrepare(GlobalTransaction gxact);
extern bool StandbyTransactionIdIsPrepared(TransactionId xid);

extern TransactionId PrescanPreparedTransactions(TransactionId **xids_p,
												 int *nxids_p);
extern void ParsePrepareRecord(uint8 info, char *xlrec,
							   xl_xact_parsed_prepare *parsed);
extern void StandbyRecoverPreparedTransactions(void);
extern void RecoverPreparedTransactions(void);

extern void CheckPointTwoPhase(XLogRecPtr redo_horizon);

extern bool FinishPreparedTransaction(const char *gid, bool isCommit, bool raiseErrorIfNotFound);

extern void PrepareRedoAdd(char *buf, XLogRecPtr start_lsn,
						   XLogRecPtr end_lsn, RepOriginId origin_id);
extern void PrepareRedoRemove(TransactionId xid, bool giveWarning);
extern void restoreTwoPhaseData(void);
#endif							/* TWOPHASE_H */
