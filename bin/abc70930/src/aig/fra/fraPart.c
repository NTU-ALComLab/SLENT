/**CFile****************************************************************

  FileName    [fraPart.c]

  SystemName  [ABC: Logic synthesis and verification system.]

  PackageName [New FRAIG package.]

  Synopsis    [Partitioning for induction.]

  Author      [Alan Mishchenko]
  
  Affiliation [UC Berkeley]

  Date        [Ver. 1.0. Started - June 30, 2007.]

  Revision    [$Id: fraPart.c,v 1.00 2007/06/30 00:00:00 alanmi Exp $]

***********************************************************************/

#include "fra.h"

////////////////////////////////////////////////////////////////////////
///                        DECLARATIONS                              ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////

/**Function*************************************************************

  Synopsis    []

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
void Fra_ManPartitionTest( Aig_Man_t * p, int nComLim )
{
    Bar_Progress_t * pProgress;
    Vec_Vec_t * vSupps, * vSuppsIn;
    Vec_Ptr_t * vSuppsNew;
    Vec_Int_t * vSupNew, * vSup, * vSup2, * vTemp;//, * vSupIn;
    Vec_Int_t * vOverNew, * vQuantNew;
    Aig_Obj_t * pObj;
    int i, k, nCommon, CountOver, CountQuant;
    int nTotalSupp, nTotalSupp2, Entry, Largest;//, iVar;
    double Ratio, R;
    int clk;

    nTotalSupp = 0;
    nTotalSupp2 = 0;
    Ratio = 0.0;
 
    // compute supports
clk = clock();
    vSupps = (Vec_Vec_t *)Aig_ManSupports( p );
PRT( "Supports", clock() - clk );
    // remove last entry
    Aig_ManForEachPo( p, pObj, i )
    {
        vSup = Vec_VecEntry( vSupps, i );
        Vec_IntPop( vSup );
        // remember support
//        pObj->pNext = (Aig_Obj_t *)vSup;
    }

    // create reverse supports
clk = clock();
    vSuppsIn = Vec_VecStart( Aig_ManPiNum(p) );
    Aig_ManForEachPo( p, pObj, i )
    {
        vSup = Vec_VecEntry( vSupps, i );
        Vec_IntForEachEntry( vSup, Entry, k )
            Vec_VecPush( vSuppsIn, Entry, (void *)i );
    }
PRT( "Inverse ", clock() - clk );

clk = clock();
    // compute extended supports
    Largest = 0;
    vSuppsNew = Vec_PtrAlloc( Aig_ManPoNum(p) );
    vOverNew  = Vec_IntAlloc( Aig_ManPoNum(p) );
    vQuantNew = Vec_IntAlloc( Aig_ManPoNum(p) );
    pProgress = Bar_ProgressStart( stdout, Aig_ManPoNum(p) );
    Aig_ManForEachPo( p, pObj, i )
    {
        Bar_ProgressUpdate( pProgress, i, NULL );
        // get old supports
        vSup = Vec_VecEntry( vSupps, i );
        if ( Vec_IntSize(vSup) < 2 )
            continue;
        // compute new supports
        CountOver = CountQuant = 0;
        vSupNew = Vec_IntDup( vSup );
        // go through the nodes where the first var appears
        Aig_ManForEachPo( p, pObj, k )
//        iVar = Vec_IntEntry( vSup, 0 );
//        vSupIn = Vec_VecEntry( vSuppsIn, iVar );
//        Vec_IntForEachEntry( vSupIn, Entry, k )
        {
//            pObj = Aig_ManObj( p, Entry );
            // get support of this output
//            vSup2 = (Vec_Int_t *)pObj->pNext;
            vSup2 = Vec_VecEntry( vSupps, k );
            // count the number of common vars
            nCommon = Vec_IntTwoCountCommon(vSup, vSup2);
            if ( nCommon < 2 )
                continue;
            if ( nCommon > nComLim )
            {
                vSupNew = Vec_IntTwoMerge( vTemp = vSupNew, vSup2 );
                Vec_IntFree( vTemp );
                CountOver++;
            }
            else
                CountQuant++;
        }
        // save the results
        Vec_PtrPush( vSuppsNew, vSupNew );
        Vec_IntPush( vOverNew, CountOver );
        Vec_IntPush( vQuantNew, CountQuant );

        if ( Largest < Vec_IntSize(vSupNew) )
            Largest = Vec_IntSize(vSupNew);

        nTotalSupp  += Vec_IntSize(vSup);
        nTotalSupp2 += Vec_IntSize(vSupNew);
        if ( Vec_IntSize(vSup) )
            R = Vec_IntSize(vSupNew) / Vec_IntSize(vSup);
        else
            R = 0;
        Ratio += R;

        if ( R < 5.0 )
            continue;

        printf( "%6d : ", i );
        printf( "S = %5d. ", Vec_IntSize(vSup) );
        printf( "SNew = %5d. ", Vec_IntSize(vSupNew) );
        printf( "R = %7.2f. ", R );
        printf( "Over = %5d. ", CountOver );
        printf( "Quant = %5d. ", CountQuant );
        printf( "\n" );
/*
        Vec_IntForEachEntry( vSupNew, Entry, k )
            printf( "%d ", Entry );
        printf( "\n" );
*/
    }
    Bar_ProgressStop( pProgress );
PRT( "Scanning", clock() - clk );

    // print cumulative statistics
    printf( "PIs = %6d. POs = %6d. Lim = %3d.   AveS = %3d. SN = %3d. R = %4.2f Max = %5d.\n",
        Aig_ManPiNum(p), Aig_ManPoNum(p), nComLim,
        nTotalSupp/Aig_ManPoNum(p), nTotalSupp2/Aig_ManPoNum(p),
        Ratio/Aig_ManPoNum(p), Largest );

    Vec_VecFree( vSupps );
    Vec_VecFree( vSuppsIn );
    Vec_VecFree( (Vec_Vec_t *)vSuppsNew );
    Vec_IntFree( vOverNew );
    Vec_IntFree( vQuantNew );
}



/**Function*************************************************************

  Synopsis    []

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
void Fra_ManPartitionTest2( Aig_Man_t * p )
{
    Vec_Vec_t * vSupps, * vSuppsIn;
    Vec_Int_t * vSup, * vSup2, * vSup3;
    Aig_Obj_t * pObj;
    int Entry, Entry2, Entry3, Counter;
    int i, k, m, n, clk;
    char * pSupp;
 
    // compute supports
clk = clock();
    vSupps = (Vec_Vec_t *)Aig_ManSupports( p );
PRT( "Supports", clock() - clk );
    // remove last entry
    Aig_ManForEachPo( p, pObj, i )
    {
        vSup = Vec_VecEntry( vSupps, i );
        Vec_IntPop( vSup );
        // remember support
//        pObj->pNext = (Aig_Obj_t *)vSup;
    }

    // create reverse supports
clk = clock();
    vSuppsIn = Vec_VecStart( Aig_ManPiNum(p) );
    Aig_ManForEachPo( p, pObj, i )
    {
        if ( i == p->nAsserts )
            break;
        vSup = Vec_VecEntry( vSupps, i );
        Vec_IntForEachEntry( vSup, Entry, k )
            Vec_VecPush( vSuppsIn, Entry, (void *)i );
    }
PRT( "Inverse ", clock() - clk );

    // create affective supports
clk = clock();
    pSupp = ALLOC( char, Aig_ManPiNum(p) );
    Aig_ManForEachPo( p, pObj, i )
    {
        if ( i % 50 != 0 )
            continue;
        vSup = Vec_VecEntry( vSupps, i );
        memset( pSupp, 0, sizeof(char) * Aig_ManPiNum(p) );
        // go through each input of this output
        Vec_IntForEachEntry( vSup, Entry, k )
        {
            pSupp[Entry] = 1;
            vSup2 = Vec_VecEntry( vSuppsIn, Entry );
            // go though each assert of this input
            Vec_IntForEachEntry( vSup2, Entry2, m )
            {
                vSup3 = Vec_VecEntry( vSupps, Entry2 );
                // go through each input of this assert
                Vec_IntForEachEntry( vSup3, Entry3, n )
                {
                    pSupp[Entry3] = 1;
                }
            }
        }
        // count the entries
        Counter = 0;
        for ( m = 0; m < Aig_ManPiNum(p); m++ )
            Counter += pSupp[m];
        printf( "%d(%d) ", Vec_IntSize(vSup), Counter );
    }
    printf( "\n" );
PRT( "Extension ", clock() - clk );

    free( pSupp );
    Vec_VecFree( vSupps );
    Vec_VecFree( vSuppsIn );
}


////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////


