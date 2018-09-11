/**CFile****************************************************************

  FileName    [fraInd.c]

  SystemName  [ABC: Logic synthesis and verification system.]

  PackageName [New FRAIG package.]

  Synopsis    [Inductive prover.]

  Author      [Alan Mishchenko]
  
  Affiliation [UC Berkeley]

  Date        [Ver. 1.0. Started - June 30, 2007.]

  Revision    [$Id: fraInd.c,v 1.00 2007/06/30 00:00:00 alanmi Exp $]

***********************************************************************/

#include "fra.h"
#include "cnf.h"
#include "dar.h"

////////////////////////////////////////////////////////////////////////
///                        DECLARATIONS                              ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////

/**Function*************************************************************

  Synopsis    [Performs AIG rewriting on the constaint manager.]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
void Fra_FraigInductionRewrite( Fra_Man_t * p )
{
    Aig_Man_t * pTemp;
    Aig_Obj_t * pObj, * pObjPo;
    int nTruePis, k, i, clk = clock();
    // perform AIG rewriting on the speculated frames
    pTemp = Aig_ManDup( p->pManFraig, 0 );
//    pTemp = Dar_ManRwsat( pTemp, 1, 0 );
    pTemp = Dar_ManRewriteDefault( pTemp );
//    printf( "Before = %6d.  After = %6d.\n", Aig_ManNodeNum(p->pManFraig), Aig_ManNodeNum(pTemp) ); 
//Aig_ManDumpBlif( p->pManFraig, "1.blif" );
//Aig_ManDumpBlif( pTemp, "2.blif" );
//    Fra_FramesWriteCone( pTemp );
//    Aig_ManStop( pTemp );
    // transfer PI/register pointers
    assert( p->pManFraig->nRegs == pTemp->nRegs );
    assert( p->pManFraig->nAsserts == pTemp->nAsserts );
    nTruePis = Aig_ManPiNum(p->pManAig) - Aig_ManRegNum(p->pManAig);
    memset( p->pMemFraig, 0, sizeof(Aig_Obj_t *) * p->nSizeAlloc * p->nFramesAll );
    Fra_ObjSetFraig( Aig_ManConst1(p->pManAig), p->pPars->nFramesK, Aig_ManConst1(pTemp) );
    Aig_ManForEachPiSeq( p->pManAig, pObj, i )
        Fra_ObjSetFraig( pObj, p->pPars->nFramesK, Aig_ManPi(pTemp,nTruePis*p->pPars->nFramesK+i) );
    k = 0;
    assert( Aig_ManRegNum(p->pManAig) == Aig_ManPoNum(pTemp) - pTemp->nAsserts );
    Aig_ManForEachLoSeq( p->pManAig, pObj, i )
    {
        pObjPo = Aig_ManPo(pTemp, pTemp->nAsserts + k++);
        Fra_ObjSetFraig( pObj, p->pPars->nFramesK, Aig_ObjChild0(pObjPo) );
    }
    // exchange
    Aig_ManStop( p->pManFraig );
    p->pManFraig = pTemp;
p->timeRwr += clock() - clk;
}

/**Function*************************************************************

  Synopsis    [Performs speculative reduction for one node.]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
static inline void Fra_FramesConstrainNode( Aig_Man_t * pManFraig, Aig_Obj_t * pObj, int iFrame )
{
    Aig_Obj_t * pObjNew, * pObjNew2, * pObjRepr, * pObjReprNew, * pMiter;
    // skip nodes without representative
    if ( (pObjRepr = Fra_ClassObjRepr(pObj)) == NULL )
        return;
    assert( pObjRepr->Id < pObj->Id );
    // get the new node 
    pObjNew = Fra_ObjFraig( pObj, iFrame );
    // get the new node of the representative
    pObjReprNew = Fra_ObjFraig( pObjRepr, iFrame );
    // if this is the same node, no need to add constraints
    if ( Aig_Regular(pObjNew) == Aig_Regular(pObjReprNew) )
        return;
    // these are different nodes - perform speculative reduction
    pObjNew2 = Aig_NotCond( pObjReprNew, pObj->fPhase ^ pObjRepr->fPhase );
    // set the new node
    Fra_ObjSetFraig( pObj, iFrame, pObjNew2 );
    // add the constraint
    pMiter = Aig_Exor( pManFraig, Aig_Regular(pObjNew), Aig_Regular(pObjReprNew) );
    pMiter = Aig_NotCond( pMiter, Aig_Regular(pMiter)->fPhase ^ Aig_IsComplement(pMiter) );
    pMiter = Aig_Not( pMiter );
    Aig_ObjCreatePo( pManFraig, pMiter );
}

/**Function*************************************************************

  Synopsis    [Prepares the inductive case with speculative reduction.]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
Aig_Man_t * Fra_FramesWithClasses( Fra_Man_t * p )
{
    Aig_Man_t * pManFraig;
    Aig_Obj_t * pObj, * pObjLi, * pObjLo, * pObjNew;
    int i, k, f;
    assert( p->pManFraig == NULL );
    assert( Aig_ManRegNum(p->pManAig) > 0 );
    assert( Aig_ManRegNum(p->pManAig) < Aig_ManPiNum(p->pManAig) );

    // start the fraig package
    pManFraig = Aig_ManStart( Aig_ManObjNumMax(p->pManAig) * p->nFramesAll );
    pManFraig->pName = Aig_UtilStrsav( p->pManAig->pName );
    pManFraig->nRegs = p->pManAig->nRegs;
    // create PI nodes for the frames
    for ( f = 0; f < p->nFramesAll; f++ )
        Fra_ObjSetFraig( Aig_ManConst1(p->pManAig), f, Aig_ManConst1(pManFraig) );
    for ( f = 0; f < p->nFramesAll; f++ )
        Aig_ManForEachPiSeq( p->pManAig, pObj, i )
            Fra_ObjSetFraig( pObj, f, Aig_ObjCreatePi(pManFraig) );
    // create latches for the first frame
    Aig_ManForEachLoSeq( p->pManAig, pObj, i )
        Fra_ObjSetFraig( pObj, 0, Aig_ObjCreatePi(pManFraig) );

    // add timeframes
//    pManFraig->fAddStrash = 1;
    for ( f = 0; f < p->nFramesAll - 1; f++ )
    {
        // set the constraints on the latch outputs
        Aig_ManForEachLoSeq( p->pManAig, pObj, i )
            Fra_FramesConstrainNode( pManFraig, pObj, f );
        // add internal nodes of this frame
        Aig_ManForEachNode( p->pManAig, pObj, i )
        {
            pObjNew = Aig_And( pManFraig, Fra_ObjChild0Fra(pObj,f), Fra_ObjChild1Fra(pObj,f) );
            Fra_ObjSetFraig( pObj, f, pObjNew );
            Fra_FramesConstrainNode( pManFraig, pObj, f );
        }
        // transfer latch input to the latch outputs 
        Aig_ManForEachLiLoSeq( p->pManAig, pObjLi, pObjLo, k )
            Fra_ObjSetFraig( pObjLo, f+1, Fra_ObjChild0Fra(pObjLi,f) );
    }
//    pManFraig->fAddStrash = 0;
    // mark the asserts
    pManFraig->nAsserts = Aig_ManPoNum(pManFraig);
    // add the POs for the latch outputs of the last frame
    Aig_ManForEachLoSeq( p->pManAig, pObj, i )
        Aig_ObjCreatePo( pManFraig, Fra_ObjFraig(pObj,p->nFramesAll-1) );

    // remove dangling nodes
    Aig_ManCleanup( pManFraig );
    // make sure the satisfying assignment is node assigned
    assert( pManFraig->pData == NULL );
    return pManFraig;
}

/**Function*************************************************************

  Synopsis    [Prepares the inductive case with speculative reduction.]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
void Fra_FramesAddMore( Aig_Man_t * p, int nFrames )
{
    Aig_Obj_t * pObj, ** pLatches;
    int i, k, f, nNodesOld;
    // set copy pointer of each object to point to itself
    Aig_ManForEachObj( p, pObj, i )
        pObj->pData = pObj;
    // iterate and add objects
    nNodesOld = Aig_ManObjNumMax(p);
    pLatches = ALLOC( Aig_Obj_t *, Aig_ManRegNum(p) );
    for ( f = 0; f < nFrames; f++ )
    {
        // clean latch inputs and outputs
        Aig_ManForEachLiSeq( p, pObj, i )
            pObj->pData = NULL;
        Aig_ManForEachLoSeq( p, pObj, i )
            pObj->pData = NULL;
        // save the latch input values
        k = 0;
        Aig_ManForEachLiSeq( p, pObj, i )
        {
            if ( Aig_ObjFanin0(pObj)->pData )
                pLatches[k++] = Aig_ObjChild0Copy(pObj);
            else
                pLatches[k++] = NULL;
        }
        // insert them as the latch output values
        k = 0;
        Aig_ManForEachLoSeq( p, pObj, i )
            pObj->pData = pLatches[k++];
        // create the next time frame of nodes
        Aig_ManForEachNode( p, pObj, i )
        {
            if ( i > nNodesOld )
                break;
            if ( Aig_ObjFanin0(pObj)->pData && Aig_ObjFanin1(pObj)->pData )
                pObj->pData = Aig_And( p, Aig_ObjChild0Copy(pObj), Aig_ObjChild1Copy(pObj) );
            else
                pObj->pData = NULL;
        }
    }
    free( pLatches );
}

/**Function*************************************************************

  Synopsis    [Performs choicing of the AIG.]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
Aig_Man_t * Fra_FraigInduction( Aig_Man_t * pManAig, int nFramesP, int nFramesK, int nMaxImps, int fRewrite, int fUseImps, int fLatchCorr, int fWriteImps, int fVerbose, int * pnIter )
{
    int fUseSimpleCnf = 0;
    int fUseOldSimulation = 0;
    // other paramaters affecting performance
    // - presence of FRAIGing in Abc_NtkDarSeqSweep()
    // - using distance-1 patterns in Fra_SmlAssignDist1()
    // - the number of simulation patterns
    // - the number of BMC frames

    Fra_Man_t * p;
    Fra_Par_t Pars, * pPars = &Pars; 
    Aig_Obj_t * pObj;
    Cnf_Dat_t * pCnf;
    Aig_Man_t * pManAigNew;
    int nNodesBeg, nRegsBeg;
    int nIter, i, clk = clock(), clk2;

    if ( Aig_ManNodeNum(pManAig) == 0 )
    {
        if ( pnIter ) *pnIter = 0;
        return Aig_ManDup(pManAig, 1);
    }
    assert( Aig_ManLatchNum(pManAig) == 0 );
    assert( Aig_ManRegNum(pManAig) > 0 );
    assert( nFramesK > 0 );
//Aig_ManShow( pManAig, 0, NULL );
 
    nNodesBeg = Aig_ManNodeNum(pManAig);
    nRegsBeg  = Aig_ManRegNum(pManAig);

    // enhance the AIG by adding timeframes
//    Fra_FramesAddMore( pManAig, 3 );

    // get parameters
    Fra_ParamsDefaultSeq( pPars );
    pPars->nFramesP   = nFramesP;
    pPars->nFramesK   = nFramesK;
    pPars->nMaxImps   = nMaxImps;
    pPars->fVerbose   = fVerbose;
    pPars->fRewrite   = fRewrite;
    pPars->fLatchCorr = fLatchCorr;
    pPars->fUseImps   = fUseImps;
    pPars->fWriteImps = fWriteImps;

    // start the fraig manager for this run
    p = Fra_ManStart( pManAig, pPars );
    // derive and refine e-classes using K initialized frames
    if ( fUseOldSimulation )
    {
        if ( pPars->nFramesP > 0 )
        {
            pPars->nFramesP = 0;
            printf( "Fra_FraigInduction(): Prefix cannot be used.\n" );
        }
        p->pSml = Fra_SmlStart( pManAig, 0, pPars->nFramesK + 1, pPars->nSimWords );
        Fra_SmlSimulate( p, 1 );
    }
    else
    {
        // bug:  r iscas/blif/s5378.blif    ; st; ssw -v
        // bug:  r iscas/blif/s1238.blif    ; st; ssw -v
        // refine the classes with more simulation rounds
if ( fVerbose )
printf( "Simulating %d AIG nodes for %d cycles ... ", Aig_ManNodeNum(pManAig), pPars->nFramesP + 32 );
        p->pSml = Fra_SmlSimulateSeq( pManAig, pPars->nFramesP, 32, 1 ); //pPars->nFramesK + 1, 1 );  
if ( fVerbose ) 
{
PRT( "Time", clock() - clk );
}
        Fra_ClassesPrepare( p->pCla, p->pPars->fLatchCorr );
//        Fra_ClassesPostprocess( p->pCla );
        // allocate new simulation manager for simulating counter-examples
        Fra_SmlStop( p->pSml );
        p->pSml = Fra_SmlStart( pManAig, 0, pPars->nFramesK + 1, pPars->nSimWords );
    }

    // select the most expressive implications
    if ( pPars->fUseImps )
        p->pCla->vImps = Fra_ImpDerive( p, 5000000, pPars->nMaxImps, pPars->fLatchCorr );

    // perform BMC (for the min number of frames)
    Fra_BmcPerform( p, pPars->nFramesP, pPars->nFramesK+1 ); // +1 is needed to prevent non-refinement
//Fra_ClassesPrint( p->pCla, 1 );
//    if ( p->vCex == NULL )
//        p->vCex = Vec_IntAlloc( 1000 );

    p->nLitsBeg  = Fra_ClassesCountLits( p->pCla );
    p->nNodesBeg = nNodesBeg; // Aig_ManNodeNum(pManAig);
    p->nRegsBeg  = nRegsBeg; // Aig_ManRegNum(pManAig);

    // dump AIG of the timeframes
//    pManAigNew = Fra_ClassesDeriveAig( p->pCla, pPars->nFramesK );
//    Aig_ManDumpBlif( pManAigNew, "frame_aig.blif" );
//    Fra_ManPartitionTest2( pManAigNew );
//    Aig_ManStop( pManAigNew );

    // iterate the inductive case
    p->pCla->fRefinement = 1;
    for ( nIter = 0; p->pCla->fRefinement; nIter++ )
    {
        int nLitsOld = Fra_ClassesCountLits(p->pCla);
        int nImpsOld = p->pCla->vImps? Vec_IntSize(p->pCla->vImps) : 0;
        // mark the classes as non-refined
        p->pCla->fRefinement = 0;
        // derive non-init K-timeframes while implementing e-classes
clk2 = clock();
        p->pManFraig = Fra_FramesWithClasses( p );
p->timeTrav += clock() - clk2;
//Aig_ManDumpBlif( p->pManFraig, "testaig.blif" );

        // perform AIG rewriting
        if ( p->pPars->fRewrite )
            Fra_FraigInductionRewrite( p );

        // convert the manager to SAT solver (the last nLatches outputs are inputs)
        if ( fUseSimpleCnf || pPars->fUseImps )
            pCnf = Cnf_DeriveSimple( p->pManFraig, Aig_ManRegNum(p->pManFraig) );
        else
            pCnf = Cnf_Derive( p->pManFraig, Aig_ManRegNum(p->pManFraig) );
//Cnf_DataWriteIntoFile( pCnf, "temp.cnf", 1 );

        p->pSat = Cnf_DataWriteIntoSolver( pCnf );
        p->nSatVars = pCnf->nVars;
        assert( p->pSat != NULL );
        if ( p->pSat == NULL )
            printf( "Fra_FraigInduction(): Computed CNF is not valid.\n" );
        if ( pPars->fUseImps )
        {
            Fra_ImpAddToSolver( p, p->pCla->vImps, pCnf->pVarNums );
            if ( p->pSat == NULL )
                printf( "Fra_FraigInduction(): Adding implicationsn to CNF led to a conflict.\n" );
        }
 
        // set the pointers to the manager
        Aig_ManForEachObj( p->pManFraig, pObj, i )
            pObj->pData = p;

        // prepare solver for fraiging the last timeframe
        Fra_ManClean( p, Aig_ManObjNumMax(p->pManFraig) + Aig_ManNodeNum(p->pManAig) );

        // transfer PI/LO variable numbers
        Aig_ManForEachObj( p->pManFraig, pObj, i )
        {
            if ( pCnf->pVarNums[pObj->Id] == -1 )
                continue;
            Fra_ObjSetSatNum( pObj, pCnf->pVarNums[pObj->Id] );
            Fra_ObjSetFaninVec( pObj, (void *)1 );
        }
        Cnf_DataFree( pCnf );

        // report the intermediate results
        if ( fVerbose )
        {
            printf( "%3d : Const = %6d. Class = %6d.  L = %6d. LR = %6d.  ", 
                nIter, Vec_PtrSize(p->pCla->vClasses1), Vec_PtrSize(p->pCla->vClasses), 
                Fra_ClassesCountLits(p->pCla), p->pManFraig->nAsserts );
            if ( p->pCla->vImps )
                printf( "I = %6d. ", Vec_IntSize(p->pCla->vImps) );
            printf( "NR = %6d.\n", Aig_ManNodeNum(p->pManFraig) );
        } 

        // perform sweeping
        p->nSatCallsRecent = 0;
        p->nSatCallsSkipped = 0;
        Fra_FraigSweep( p );

//        Sat_SolverPrintStats( stdout, p->pSat );

        // remove FRAIG and SAT solver
        Aig_ManStop( p->pManFraig );   p->pManFraig = NULL;
        sat_solver_delete( p->pSat );  p->pSat = NULL; 
        memset( p->pMemFraig, 0, sizeof(Aig_Obj_t *) * p->nSizeAlloc * p->nFramesAll );
//        printf( "Recent SAT called = %d. Skipped = %d.\n", p->nSatCallsRecent, p->nSatCallsSkipped );
        assert( p->vTimeouts == NULL );
        if ( p->vTimeouts )
           printf( "Fra_FraigInduction(): SAT solver timed out!\n" );
        // check if refinement has happened
//        p->pCla->fRefinement = (int)(nLitsOld != Fra_ClassesCountLits(p->pCla));
        if ( p->pCla->fRefinement && 
            nLitsOld == Fra_ClassesCountLits(p->pCla) && 
            nImpsOld == (p->pCla->vImps? Vec_IntSize(p->pCla->vImps) : 0) )
        {
            printf( "Fra_FraigInduction(): Internal error. The result may not verify.\n" );
            break;
        }
    }
/*
    // verify implications using simulation
    if ( p->pCla->vImps && Vec_IntSize(p->pCla->vImps) )
    {
        int Temp, clk = clock();
        if ( Temp = Fra_ImpVerifyUsingSimulation( p ) )
            printf( "Implications failing the simulation test = %d (out of %d).  ", Temp, Vec_IntSize(p->pCla->vImps) );
        else
            printf( "All %d implications have passed the simulation test.  ", Vec_IntSize(p->pCla->vImps) );
        PRT( "Time", clock() - clk );
    }
*/

    // move the classes into representatives and reduce AIG
clk2 = clock();
//    Fra_ClassesPrint( p->pCla, 1 );
    Fra_ClassesSelectRepr( p->pCla );
    Fra_ClassesCopyReprs( p->pCla, p->vTimeouts );
    pManAigNew = Aig_ManDupRepr( pManAig, 0 );
    // add implications to the manager
    if ( fWriteImps && p->pCla->vImps && Vec_IntSize(p->pCla->vImps) )
        Fra_ImpRecordInManager( p, pManAigNew );
    // cleanup the new manager
    Aig_ManSeqCleanup( pManAigNew );
//    Aig_ManCountMergeRegs( pManAigNew );
p->timeTrav += clock() - clk2;
p->timeTotal = clock() - clk;
    // get the final stats
    p->nLitsEnd  = Fra_ClassesCountLits( p->pCla );
    p->nNodesEnd = Aig_ManNodeNum(pManAigNew);
    p->nRegsEnd  = Aig_ManRegNum(pManAigNew);
    // free the manager
    Fra_ManStop( p );
    // check the output
//    if ( Aig_ManPoNum(pManAigNew) - Aig_ManRegNum(pManAigNew) == 1 )
//        if ( Aig_ObjChild0( Aig_ManPo(pManAigNew,0) ) == Aig_ManConst0(pManAigNew) )
//            printf( "Proved output constant 0.\n" );
    if ( pnIter ) *pnIter = nIter;
    return pManAigNew;
}

////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////


