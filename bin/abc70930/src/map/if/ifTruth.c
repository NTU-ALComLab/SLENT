/**CFile****************************************************************

  FileName    [ifTruth.c]

  SystemName  [ABC: Logic synthesis and verification system.]

  PackageName [FPGA mapping based on priority cuts.]

  Synopsis    [Computation of truth tables of the cuts.]

  Author      [Alan Mishchenko]
  
  Affiliation [UC Berkeley]

  Date        [Ver. 1.0. Started - November 21, 2006.]

  Revision    [$Id: ifTruth.c,v 1.00 2006/11/21 00:00:00 alanmi Exp $]

***********************************************************************/

#include "if.h"

////////////////////////////////////////////////////////////////////////
///                        DECLARATIONS                              ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////

/**Function*************************************************************

  Synopsis    [Several simple procedures working with truth tables.]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
static inline int  If_TruthWordNum( int nVars )  { return nVars <= 5 ? 1 : (1 << (nVars - 5)); }
static inline void If_TruthNot( unsigned * pOut, unsigned * pIn, int nVars )
{
    int w;
    for ( w = If_TruthWordNum(nVars)-1; w >= 0; w-- )
        pOut[w] = ~pIn[w];
}
static inline void If_TruthCopy( unsigned * pOut, unsigned * pIn, int nVars )
{
    int w;
    for ( w = If_TruthWordNum(nVars)-1; w >= 0; w-- )
        pOut[w] = pIn[w];
}
static inline void If_TruthNand( unsigned * pOut, unsigned * pIn0, unsigned * pIn1, int nVars )
{
    int w;
    for ( w = If_TruthWordNum(nVars)-1; w >= 0; w-- )
        pOut[w] = ~(pIn0[w] & pIn1[w]);
}
static inline void If_TruthAnd( unsigned * pOut, unsigned * pIn0, unsigned * pIn1, int nVars )
{
    int w;
    for ( w = If_TruthWordNum(nVars)-1; w >= 0; w-- )
        pOut[w] = pIn0[w] & pIn1[w];
}

/**Function*************************************************************

  Synopsis    [Swaps two adjacent variables in the truth table.]

  Description [Swaps var number Start and var number Start+1 (0-based numbers).
  The input truth table is pIn. The output truth table is pOut.]
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
void If_TruthSwapAdjacentVars( unsigned * pOut, unsigned * pIn, int nVars, int iVar )
{
    static unsigned PMasks[4][3] = {
        { 0x99999999, 0x22222222, 0x44444444 },
        { 0xC3C3C3C3, 0x0C0C0C0C, 0x30303030 },
        { 0xF00FF00F, 0x00F000F0, 0x0F000F00 },
        { 0xFF0000FF, 0x0000FF00, 0x00FF0000 }
    };
    int nWords = If_TruthWordNum( nVars );
    int i, k, Step, Shift;

    assert( iVar < nVars - 1 );
    if ( iVar < 4 )
    {
        Shift = (1 << iVar);
        for ( i = 0; i < nWords; i++ )
            pOut[i] = (pIn[i] & PMasks[iVar][0]) | ((pIn[i] & PMasks[iVar][1]) << Shift) | ((pIn[i] & PMasks[iVar][2]) >> Shift);
    }
    else if ( iVar > 4 )
    {
        Step = (1 << (iVar - 5));
        for ( k = 0; k < nWords; k += 4*Step )
        {
            for ( i = 0; i < Step; i++ )
                pOut[i] = pIn[i];
            for ( i = 0; i < Step; i++ )
                pOut[Step+i] = pIn[2*Step+i];
            for ( i = 0; i < Step; i++ )
                pOut[2*Step+i] = pIn[Step+i];
            for ( i = 0; i < Step; i++ )
                pOut[3*Step+i] = pIn[3*Step+i];
            pIn  += 4*Step;
            pOut += 4*Step;
        }
    }
    else // if ( iVar == 4 )
    {
        for ( i = 0; i < nWords; i += 2 )
        {
            pOut[i]   = (pIn[i]   & 0x0000FFFF) | ((pIn[i+1] & 0x0000FFFF) << 16);
            pOut[i+1] = (pIn[i+1] & 0xFFFF0000) | ((pIn[i]   & 0xFFFF0000) >> 16);
        }
    }
}

/**Function*************************************************************

  Synopsis    [Expands the truth table according to the phase.]

  Description [The input and output truth tables are in pIn/pOut. The current number
  of variables is nVars. The total number of variables in nVarsAll. The last argument
  (Phase) contains shows where the variables should go.]
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
void If_TruthStretch( unsigned * pOut, unsigned * pIn, int nVars, int nVarsAll, unsigned Phase )
{
    unsigned * pTemp;
    int i, k, Var = nVars - 1, Counter = 0;
    for ( i = nVarsAll - 1; i >= 0; i-- )
        if ( Phase & (1 << i) )
        {
            for ( k = Var; k < i; k++ )
            {
                If_TruthSwapAdjacentVars( pOut, pIn, nVarsAll, k );
                pTemp = pIn; pIn = pOut; pOut = pTemp;
                Counter++;
            }
            Var--;
        }
    assert( Var == -1 );
    // swap if it was moved an even number of times
    if ( !(Counter & 1) )
        If_TruthCopy( pOut, pIn, nVarsAll );
}

/**Function*************************************************************

  Synopsis    [Computes the stretching phase of the cut w.r.t. the merged cut.]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
static inline unsigned If_CutTruthPhase( If_Cut_t * pCut, If_Cut_t * pCut1 )
{
    unsigned uPhase = 0;
    int i, k;
    for ( i = k = 0; i < (int)pCut->nLeaves; i++ )
    {
        if ( k == (int)pCut1->nLeaves )
            break;
        if ( pCut->pLeaves[i] < pCut1->pLeaves[k] )
            continue;
        assert( pCut->pLeaves[i] == pCut1->pLeaves[k] );
        uPhase |= (1 << i);
        k++;
    }
    return uPhase;
}

/**Function*************************************************************

  Synopsis    [Performs truth table computation.]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
void If_CutComputeTruth( If_Man_t * p, If_Cut_t * pCut, If_Cut_t * pCut0, If_Cut_t * pCut1, int fCompl0, int fCompl1 )
{
    extern void Kit_FactorTest( unsigned * pTruth, int nVars );

    // permute the first table
    if ( fCompl0 ^ pCut0->fCompl ) 
        If_TruthNot( p->puTemp[0], If_CutTruth(pCut0), pCut->nLimit );
    else
        If_TruthCopy( p->puTemp[0], If_CutTruth(pCut0), pCut->nLimit );
    If_TruthStretch( p->puTemp[2], p->puTemp[0], pCut0->nLeaves, pCut->nLimit, If_CutTruthPhase(pCut, pCut0) );
    // permute the second table
    if ( fCompl1 ^ pCut1->fCompl ) 
        If_TruthNot( p->puTemp[1], If_CutTruth(pCut1), pCut->nLimit );
    else
        If_TruthCopy( p->puTemp[1], If_CutTruth(pCut1), pCut->nLimit );
    If_TruthStretch( p->puTemp[3], p->puTemp[1], pCut1->nLeaves, pCut->nLimit, If_CutTruthPhase(pCut, pCut1) );
    // produce the resulting table
    assert( pCut->fCompl == 0 );
    if ( pCut->fCompl )
        If_TruthNand( If_CutTruth(pCut), p->puTemp[2], p->puTemp[3], pCut->nLimit );
    else
        If_TruthAnd( If_CutTruth(pCut), p->puTemp[2], p->puTemp[3], pCut->nLimit );

    // perform 
//    Kit_FactorTest( If_CutTruth(pCut), pCut->nLimit );
//    printf( "%d ", If_CutLeaveNum(pCut) - Kit_TruthSupportSize(If_CutTruth(pCut), If_CutLeaveNum(pCut)) );
}

////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////


