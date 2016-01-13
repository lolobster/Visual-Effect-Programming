//--------------------------------------------------------------------------------------
// File: FrmInput.cpp
// Desc:
//
// Author:     QUALCOMM, Advanced Content Group - Snapdragon SDK
//
//               Copyright (c) 2013 QUALCOMM Technologies, Inc. 
//                         All Rights Reserved. 
//                      QUALCOMM Proprietary/GTDR
//--------------------------------------------------------------------------------------
#include <FrmPlatform.h>
#include <graphics/FrmInput.h>


//--------------------------------------------------------------------------------------
// Name: GetButtonState()
// Desc: Process button input that changed since the last time this function was called.
//--------------------------------------------------------------------------------------
VOID FRM_INPUT::GetButtonState( UINT32* pnButtons, UINT32* pnPressedButtons )
{
    m_nPressedButtons = m_nButtons & (~m_nLastButtons);
    m_nLastButtons    = m_nButtons;

    if( pnPressedButtons )  (*pnPressedButtons)  = m_nPressedButtons;
    if( pnButtons )         (*pnButtons)         = m_nButtons;
}


//--------------------------------------------------------------------------------------
// Name: GetPointerState()
// Desc: Process pointer input that changed since the last time this function was called.
//--------------------------------------------------------------------------------------
VOID FRM_INPUT::GetPointerState( UINT32* pnPointerState, FRMVECTOR2* pvPointerPosition )
{
    if( pnPointerState )    (*pnPointerState)    = m_nPointerState;
    if( pvPointerPosition ) (*pvPointerPosition) = m_vPointerPosition;

    // Reset any one-time pointer state
    m_nPointerState &= ~FRM_INPUT::POINTER_PRESSED;
    m_nPointerState &= ~FRM_INPUT::POINTER_RELEASED;
}


//--------------------------------------------------------------------------------------
// Name: FrmGetInput()
// Desc: Process input that changed since the last time this function was called.
//--------------------------------------------------------------------------------------
VOID FrmGetInput( FRM_INPUT* pInput, UINT32* pnButtons, UINT32* pnPressedButtons )
{
    pInput->GetButtonState( pnButtons, pnPressedButtons );
}

