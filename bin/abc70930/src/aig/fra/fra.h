/**CFile****************************************************************

  FileName    [fra.h]

  SystemName  [ABC: Logic synthesis and verification system.]

  PackageName [[New FRAIG package.]

  Synopsis    [External declarations.]

  Author      [Alan Mishchenko]
  
  Affiliation [UC Berkeley]

  Date        [Ver. 1.0. Started - June 30, 2007.]

  Revision    [$Id: fra.h,v 1.00 2007/06/30 00:00:00 alanmi Exp $]

***********************************************************************/

#ifndef __FRA_H__
#define __FRA_H__

#ifdef __cplusplus
extern "C" {
#endif 

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#include "vec.h"
#include "aig.h"
#include "dar.h"
#include "satSolver.h"
#include "bar.h"

////////////////////////////////////////////////////////////////////////
///                         PARAMETERS                               ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                         BASIC TYPES                              ///
////////////////////////////////////////////////////////////////////////

typedef struct Fra_Par_t_   Fra_Par_t;
typedef struct Fra_Man_t_   Fra_Man_t;
typedef struct Fra_Cla_t_   Fra_Cla_t;
typedef struct Fra_Sml_t_   Fra_Sml_t;
typedef struct Fra_Bmc_t_   Fra_Bmc_t;

// FRAIG parameters
struct Fra_Par_t_
{
    int              nSimWords;         // the number of words in the simulation info
    double           dSimSatur;         // the ratio of refined classes when saturation is reached
    int              fPatScores;        // enables simulation pattern scoring
    int              MaxScore;          // max score after which resimulation is used
    double           dActConeRatio;     // the ratio of cone to be bumped
    double           dActConeBumpMax;   // the largest bump in activity
    int              fChoicing;         // enables choicing
    int              fSpeculate;        // use speculative reduction
    int              fProve;            // prove the miter outputs
    int              fVerbose;          // verbose output
    int              fDoSparse;         // skip sparse functions
    int              fConeBias;         // bias variables in the cone (good for unsat runs)
    int              nBTLimitNode;      // conflict limit at a node
    int              nBTLimitMiter;     // conflict limit at an output
    int              nFramesP;          // the number of timeframes to in the prefix
    int              nFramesK;          // the number of timeframes to unroll
    int              nMaxImps;          // the maximum number of implications to consider
    int              fRewrite;          // use rewriting for constraint reduction
    int              fLatchCorr;        // computes latch correspondence only
    int              fUseImps;          // use implications
    int              fWriteImps;        // record implications
};

// FRAIG equivalence classes
struct Fra_Cla_t_
{
    Aig_Man_t *      pAig;              // the original AIG manager
    Aig_Obj_t **     pMemRepr;          // pointers to representatives of each node
    Vec_Ptr_t *      vClasses;          // equivalence classes
    Vec_Ptr_t *      vClasses1;         // equivalence class of Const1 node
    Vec_Ptr_t *      vClassesTemp;      // temporary storage for new classes
    Aig_Obj_t **     pMemClasses;       // memory allocated for equivalence classes
    Aig_Obj_t **     pMemClassesFree;   // memory allocated for equivalence classes to be used
    Vec_Ptr_t *      vClassOld;         // old equivalence class after splitting
    Vec_Ptr_t *      vClassNew;         // new equivalence class(es) after splitting
    int              nPairs;            // the number of pairs of nodes
    int              fRefinement;       // set to 1 when refinement has happened
    Vec_Int_t *      vImps;             // implications
    // procedures used for class refinement
    int (*pFuncNodeHash)     (Aig_Obj_t *, int);         // returns has key of the node
    int (*pFuncNodeIsConst)  (Aig_Obj_t *);              // returns 1 if the node is a constant
    int (*pFuncNodesAreEqual)(Aig_Obj_t *, Aig_Obj_t *); // returns 1 if nodes are equal up to a complement
};

// simulation manager
struct Fra_Sml_t_
{
    Aig_Man_t *      pAig;              // the original AIG manager
    int              nPref;             // the number of times frames in the prefix
    int              nFrames;           // the number of times frames 
    int              nWordsFrame;       // the number of words in each time frame
    int              nWordsTotal;       // the total number of words at a node
    int              nWordsPref;        // the number of word in the prefix
    int              fNonConstOut;      // have seen a non-const-0 output during simulation
    int              nSimRounds;        // statistics
    int              timeSim;           // statistics
    unsigned         pData[0];          // simulation data for the nodes
};

// FRAIG manager
struct Fra_Man_t_
{
    // high-level data    
    Fra_Par_t *      pPars;             // parameters governing fraiging
    // AIG managers
    Aig_Man_t *      pManAig;           // the starting AIG manager
    Aig_Man_t *      pManFraig;         // the final AIG manager
    // mapping AIG into FRAIG
    int              nFramesAll;        // the number of timeframes used
    Aig_Obj_t **     pMemFraig;         // memory allocated for points to the fraig nodes
    int              nSizeAlloc;        // allocated size of the arrays for timeframe nodes
    // equivalence classes 
    Fra_Cla_t *      pCla;              // representation of (candidate) equivalent nodes
    // simulation info
    Fra_Sml_t *      pSml;              // simulation manager
    // bounded model checking manager
    Fra_Bmc_t *      pBmc;
    // counter example storage
    int              nPatWords;         // the number of words in the counter example
    unsigned *       pPatWords;         // the counter example
    Vec_Int_t *      vCex;
    // satisfiability solving
    sat_solver *     pSat;              // SAT solver
    int              nSatVars;          // the number of variables currently used
    Vec_Ptr_t *      vPiVars;           // the PIs of the cone used 
    sint64           nBTLimitGlobal;    // resource limit
    sint64           nInsLimitGlobal;   // resource limit
    Vec_Ptr_t **     pMemFanins;        // the arrays of fanins for some FRAIG nodes
    int *            pMemSatNums;       // the array of SAT numbers for some FRAIG nodes
    int              nMemAlloc;         // allocated size of the arrays for FRAIG varnums and fanins
    Vec_Ptr_t *      vTimeouts;         // the nodes, for which equivalence checking timed out
    // statistics
    int              nSimRounds;
    int              nNodesMiter;
    int              nLitsBeg;
    int              nLitsEnd;
    int              nNodesBeg;
    int              nNodesEnd;
    int              nRegsBeg;
    int              nRegsEnd;
    int              nSatCalls;
    int              nSatCallsSat;
    int              nSatCallsUnsat;
    int              nSatProof;
    int              nSatFails;
    int              nSatFailsReal;
    int              nSpeculs;   
    int              nChoices;
    int              nChoicesFake;
    int              nSatCallsRecent;
    int              nSatCallsSkipped;
    // runtime
    int              timeSim;
    int              timeTrav;
    int              timeRwr;
    int              timeSat;
    int              timeSatUnsat;
    int              timeSatSat;
    int              timeSatFail;
    int              timeRef;
    int              timeTotal;
    int              time1;
    int              time2;
};

////////////////////////////////////////////////////////////////////////
///                      MACRO DEFINITIONS                           ///
////////////////////////////////////////////////////////////////////////

static inline unsigned *   Fra_ObjSim( Fra_Sml_t * p, int Id )                           { return p->pData + p->nWordsTotal * Id; }
static inline unsigned     Fra_ObjRandomSim()                                            { return (rand() << 24) ^ (rand() << 12) ^ rand();                                                   }

static inline Aig_Obj_t *  Fra_ObjFraig( Aig_Obj_t * pObj, int i )                       { return ((Fra_Man_t *)pObj->pData)->pMemFraig[((Fra_Man_t *)pObj->pData)->nFramesAll*pObj->Id + i];  }
static inline void         Fra_ObjSetFraig( Aig_Obj_t * pObj, int i, Aig_Obj_t * pNode ) { ((Fra_Man_t *)pObj->pData)->pMemFraig[((Fra_Man_t *)pObj->pData)->nFramesAll*pObj->Id + i] = pNode; }

static inline Vec_Ptr_t *  Fra_ObjFaninVec( Aig_Obj_t * pObj )                           { return ((Fra_Man_t *)pObj->pData)->pMemFanins[pObj->Id];      }
static inline void         Fra_ObjSetFaninVec( Aig_Obj_t * pObj, Vec_Ptr_t * vFanins )   { ((Fra_Man_t *)pObj->pData)->pMemFanins[pObj->Id] = vFanins;   }

static inline int          Fra_ObjSatNum( Aig_Obj_t * pObj )                             { return ((Fra_Man_t *)pObj->pData)->pMemSatNums[pObj->Id];     }
static inline void         Fra_ObjSetSatNum( Aig_Obj_t * pObj, int Num )                 { ((Fra_Man_t *)pObj->pData)->pMemSatNums[pObj->Id] = Num;      }

static inline Aig_Obj_t *  Fra_ClassObjRepr( Aig_Obj_t * pObj )                          { return ((Fra_Man_t *)pObj->pData)->pCla->pMemRepr[pObj->Id];  }
static inline void         Fra_ClassObjSetRepr( Aig_Obj_t * pObj, Aig_Obj_t * pNode )    { ((Fra_Man_t *)pObj->pData)->pCla->pMemRepr[pObj->Id] = pNode; }

static inline Aig_Obj_t *  Fra_ObjChild0Fra( Aig_Obj_t * pObj, int i ) { assert( !Aig_IsComplement(pObj) ); return Aig_ObjFanin0(pObj)? Aig_NotCond(Fra_ObjFraig(Aig_ObjFanin0(pObj),i), Aig_ObjFaninC0(pObj)) : NULL;  }
static inline Aig_Obj_t *  Fra_ObjChild1Fra( Aig_Obj_t * pObj, int i ) { assert( !Aig_IsComplement(pObj) ); return Aig_ObjFanin1(pObj)? Aig_NotCond(Fra_ObjFraig(Aig_ObjFanin1(pObj),i), Aig_ObjFaninC1(pObj)) : NULL;  }

static inline int          Fra_ImpLeft( int Imp )                                        { return Imp & 0xFFFF;         }
static inline int          Fra_ImpRight( int Imp )                                       { return Imp >> 16;            }
static inline int          Fra_ImpCreate( int Left, int Right )                          { return (Right << 16) | Left; }

////////////////////////////////////////////////////////////////////////
///                         ITERATORS                                ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                    FUNCTION DECLARATIONS                         ///
////////////////////////////////////////////////////////////////////////

/*=== fraClass.c ========================================================*/
extern int                 Fra_BmcNodeIsConst( Aig_Obj_t * pObj );
extern int                 Fra_BmcNodesAreEqual( Aig_Obj_t * pObj0, Aig_Obj_t * pObj1 );
extern void                Fra_BmcStop( Fra_Bmc_t * p );
extern void                Fra_BmcPerform( Fra_Man_t * p, int nPref, int nDepth );
/*=== fraClass.c ========================================================*/
extern Fra_Cla_t *         Fra_ClassesStart( Aig_Man_t * pAig );
extern void                Fra_ClassesStop( Fra_Cla_t * p );
extern void                Fra_ClassesCopyReprs( Fra_Cla_t * p, Vec_Ptr_t * vFailed );
extern void                Fra_ClassesPrint( Fra_Cla_t * p, int fVeryVerbose );
extern void                Fra_ClassesPrepare( Fra_Cla_t * p, int fLatchCorr );
extern int                 Fra_ClassesRefine( Fra_Cla_t * p );
extern int                 Fra_ClassesRefine1( Fra_Cla_t * p, int fRefineNewClass, int * pSkipped );
extern int                 Fra_ClassesCountLits( Fra_Cla_t * p );
extern int                 Fra_ClassesCountPairs( Fra_Cla_t * p );
extern void                Fra_ClassesTest( Fra_Cla_t * p, int Id1, int Id2 );
extern void                Fra_ClassesLatchCorr( Fra_Man_t * p );
extern void                Fra_ClassesPostprocess( Fra_Cla_t * p );
extern void                Fra_ClassesSelectRepr( Fra_Cla_t * p );
extern Aig_Man_t *         Fra_ClassesDeriveAig( Fra_Cla_t * p, int nFramesK );
/*=== fraCnf.c ========================================================*/
extern void                Fra_CnfNodeAddToSolver( Fra_Man_t * p, Aig_Obj_t * pOld, Aig_Obj_t * pNew );
/*=== fraCore.c ========================================================*/
extern void                Fra_FraigSweep( Fra_Man_t * pManAig );
extern int                 Fra_FraigMiterStatus( Aig_Man_t * p );
extern Aig_Man_t *         Fra_FraigPerform( Aig_Man_t * pManAig, Fra_Par_t * pPars );
extern Aig_Man_t *         Fra_FraigChoice( Aig_Man_t * pManAig, int nConfMax );
extern Aig_Man_t *         Fra_FraigEquivence( Aig_Man_t * pManAig, int nConfMax );
/*=== fraImp.c ========================================================*/
extern Vec_Int_t *         Fra_ImpDerive( Fra_Man_t * p, int nImpMaxLimit, int nImpUseLimit, int fLatchCorr );
extern void                Fra_ImpAddToSolver( Fra_Man_t * p, Vec_Int_t * vImps, int * pSatVarNums );
extern int                 Fra_ImpCheckForNode( Fra_Man_t * p, Vec_Int_t * vImps, Aig_Obj_t * pNode, int Pos );
extern int                 Fra_ImpRefineUsingCex( Fra_Man_t * p, Vec_Int_t * vImps );
extern void                Fra_ImpCompactArray( Vec_Int_t * vImps );
extern double              Fra_ImpComputeStateSpaceRatio( Fra_Man_t * p );
extern int                 Fra_ImpVerifyUsingSimulation( Fra_Man_t * p );
extern void                Fra_ImpRecordInManager( Fra_Man_t * p, Aig_Man_t * pNew );
/*=== fraInd.c ========================================================*/
extern Aig_Man_t *         Fra_FraigInduction( Aig_Man_t * p, int nFramesP, int nFramesK, int nMaxImps, int fRewrite, int fUseImps, int fLatchCorr, int fWriteImps, int fVerbose, int * pnIter );
/*=== fraLcr.c ========================================================*/
extern Aig_Man_t *         Fra_FraigLatchCorrespondence( Aig_Man_t * pAig, int nFramesP, int nConfMax, int fProve, int fVerbose, int * pnIter );
/*=== fraMan.c ========================================================*/
extern void                Fra_ParamsDefault( Fra_Par_t * pParams );
extern void                Fra_ParamsDefaultSeq( Fra_Par_t * pParams );
extern Fra_Man_t *         Fra_ManStart( Aig_Man_t * pManAig, Fra_Par_t * pParams );
extern void                Fra_ManClean( Fra_Man_t * p, int nNodesMax );
extern Aig_Man_t *         Fra_ManPrepareComb( Fra_Man_t * p );
extern void                Fra_ManFinalizeComb( Fra_Man_t * p );
extern void                Fra_ManStop( Fra_Man_t * p );
extern void                Fra_ManPrint( Fra_Man_t * p );
/*=== fraSat.c ========================================================*/
extern int                 Fra_NodesAreEquiv( Fra_Man_t * p, Aig_Obj_t * pOld, Aig_Obj_t * pNew );
extern int                 Fra_NodesAreImp( Fra_Man_t * p, Aig_Obj_t * pOld, Aig_Obj_t * pNew, int fComplL, int fComplR );
extern int                 Fra_NodeIsConst( Fra_Man_t * p, Aig_Obj_t * pNew );
/*=== fraSec.c ========================================================*/
extern int                 Fra_FraigSec( Aig_Man_t * p, int nFrames, int fRetimeFirst, int fVerbose, int fVeryVerbose );
/*=== fraSim.c ========================================================*/
extern int                 Fra_SmlNodeHash( Aig_Obj_t * pObj, int nTableSize );
extern int                 Fra_SmlNodeIsConst( Aig_Obj_t * pObj );
extern int                 Fra_SmlNodesAreEqual( Aig_Obj_t * pObj0, Aig_Obj_t * pObj1 );
extern int                 Fra_SmlNodeNotEquWeight( Fra_Sml_t * p, int Left, int Right );
extern int                 Fra_SmlCheckOutput( Fra_Man_t * p );
extern void                Fra_SmlSavePattern( Fra_Man_t * p );
extern void                Fra_SmlSimulate( Fra_Man_t * p, int fInit );
extern void                Fra_SmlResimulate( Fra_Man_t * p );
extern Fra_Sml_t *         Fra_SmlStart( Aig_Man_t * pAig, int nPref, int nFrames, int nWordsFrame );
extern void                Fra_SmlStop( Fra_Sml_t * p );
extern Fra_Sml_t *         Fra_SmlSimulateSeq( Aig_Man_t * pAig, int nPref, int nFrames, int nWords );
extern Fra_Sml_t *         Fra_SmlSimulateComb( Aig_Man_t * pAig, int nWords );


#ifdef __cplusplus
}
#endif

#endif

////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////

