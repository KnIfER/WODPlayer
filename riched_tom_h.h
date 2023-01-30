

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 11:14:07 2038
 */
/* Compiler settings for ..\riched20\riched_tom.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0622 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */


#ifndef __riched_tom_h_h__
#define __riched_tom_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ITextRange_FWD_DEFINED__
#define __ITextRange_FWD_DEFINED__
typedef interface ITextRange ITextRange;

#endif 	/* __ITextRange_FWD_DEFINED__ */


#ifndef __ITextSelection_FWD_DEFINED__
#define __ITextSelection_FWD_DEFINED__
typedef interface ITextSelection ITextSelection;

#endif 	/* __ITextSelection_FWD_DEFINED__ */


#ifndef __ITextStoryRanges_FWD_DEFINED__
#define __ITextStoryRanges_FWD_DEFINED__
typedef interface ITextStoryRanges ITextStoryRanges;

#endif 	/* __ITextStoryRanges_FWD_DEFINED__ */


#ifndef __ITextDocument_FWD_DEFINED__
#define __ITextDocument_FWD_DEFINED__
typedef interface ITextDocument ITextDocument;

#endif 	/* __ITextDocument_FWD_DEFINED__ */


#ifndef __ITextFont_FWD_DEFINED__
#define __ITextFont_FWD_DEFINED__
typedef interface ITextFont ITextFont;

#endif 	/* __ITextFont_FWD_DEFINED__ */


#ifndef __ITextPara_FWD_DEFINED__
#define __ITextPara_FWD_DEFINED__
typedef interface ITextPara ITextPara;

#endif 	/* __ITextPara_FWD_DEFINED__ */


/* header files for imported files */
#include "unknwn.h"
#include "objidl.h"
#include "oaidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_riched_tom_0000_0000 */
/* [local] */ 

#pragma makedep regtypelib
#ifdef WINE_NO_UNICODE_MACROS
#undef FindText
#endif


extern RPC_IF_HANDLE __MIDL_itf_riched_tom_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_riched_tom_0000_0000_v0_0_s_ifspec;


#ifndef __tom_LIBRARY_DEFINED__
#define __tom_LIBRARY_DEFINED__

/* library tom */
/* [version][uuid] */ 

typedef 
enum tagTomConstants
    {
        tomFalse	= 0,
        tomTrue	= -1,
        tomUndefined	= -9999999,
        tomToggle	= -9999998,
        tomAutoColor	= -9999997,
        tomDefault	= -9999996,
        tomSuspend	= -9999995,
        tomResume	= -9999994,
        tomApplyNow	= 0,
        tomApplyLater	= 1,
        tomTrackParms	= 2,
        tomCacheParms	= 3,
        tomApplyTmp	= 4,
        tomDisableSmartFont	= 8,
        tomEnableSmartFont	= 9,
        tomUsePoints	= 10,
        tomUseTwips	= 11,
        tomBackward	= 0xc0000001,
        tomForward	= 0x3fffffff,
        tomMove	= 0,
        tomExtend	= 1,
        tomNoSelection	= 0,
        tomSelectionIP	= 1,
        tomSelectionNormal	= 2,
        tomSelectionFrame	= 3,
        tomSelectionColumn	= 4,
        tomSelectionRow	= 5,
        tomSelectionBlock	= 6,
        tomSelectionInlineShape	= 7,
        tomSelectionShape	= 8,
        tomSelStartActive	= 1,
        tomSelAtEOL	= 2,
        tomSelOvertype	= 4,
        tomSelActive	= 8,
        tomSelReplace	= 16,
        tomEnd	= 0,
        tomStart	= 32,
        tomCollapseEnd	= 0,
        tomCollapseStart	= 1,
        tomClientCoord	= 256,
        tomNone	= 0,
        tomSingle	= 1,
        tomWords	= 2,
        tomDouble	= 3,
        tomDotted	= 4,
        tomDash	= 5,
        tomDashDot	= 6,
        tomDashDotDot	= 7,
        tomWave	= 8,
        tomThick	= 9,
        tomHair	= 10,
        tomLineSpaceSingle	= 0,
        tomLineSpace1pt5	= 1,
        tomLineSpaceDouble	= 2,
        tomLineSpaceAtLeast	= 3,
        tomLineSpaceExactly	= 4,
        tomLineSpaceMultiple	= 5,
        tomAlignLeft	= 0,
        tomAlignCenter	= 1,
        tomAlignRight	= 2,
        tomAlignJustify	= 3,
        tomAlignDecimal	= 3,
        tomAlignBar	= 4,
        tomAlignInterWord	= 3,
        tomAlignInterLetter	= 4,
        tomAlignScaled	= 5,
        tomAlignGlyphs	= 6,
        tomAlignSnapGrid	= 7,
        tomSpaces	= 0,
        tomDots	= 1,
        tomDashes	= 2,
        tomLines	= 3,
        tomThickLines	= 4,
        tomEquals	= 5,
        tomTabBack	= -3,
        tomTabNext	= -2,
        tomTabHere	= -1,
        tomListBullet	= 1,
        tomListNumberAsArabic	= 2,
        tomListNumberAsLCLetter	= 3,
        tomListNumberAsUCLetter	= 4,
        tomListNumberAsLCRoman	= 5,
        tomListNumberAsUCRoman	= 6,
        tomListNumberAsSequence	= 7,
        tomListParentheses	= 0x10000,
        tomListPeriod	= 0x20000,
        tomListPlain	= 0x30000,
        tomCharacter	= 1,
        tomWord	= 2,
        tomSentence	= 3,
        tomParagraph	= 4,
        tomLine	= 5,
        tomStory	= 6,
        tomScreen	= 7,
        tomSection	= 8,
        tomColumn	= 9,
        tomRow	= 10,
        tomWindow	= 11,
        tomCell	= 12,
        tomCharFormat	= 13,
        tomParaFormat	= 14,
        tomTable	= 15,
        tomObject	= 16,
        tomMatchWord	= 2,
        tomMatchCase	= 4,
        tomMatchPattern	= 8,
        tomUnknownStory	= 0,
        tomMainTextStory	= 1,
        tomFootnotesStory	= 2,
        tomEndnotesStory	= 3,
        tomCommentsStory	= 4,
        tomTextFrameStory	= 5,
        tomEvenPagesHeaderStory	= 6,
        tomPrimaryHeaderStory	= 7,
        tomEvenPagesFooterStory	= 8,
        tomPrimaryFooterStory	= 9,
        tomFirstPageHeaderStory	= 10,
        tomFirstPageFooterStory	= 11,
        tomNoAnimation	= 0,
        tomLasVegasLights	= 1,
        tomBlinkingBackground	= 2,
        tomSparkleText	= 3,
        tomMarchingBlackAnts	= 4,
        tomMarchingRedAnts	= 5,
        tomShimmer	= 6,
        tomWipeDown	= 7,
        tomWipeRight	= 8,
        tomAnimationMax	= 8,
        tomLowerCase	= 0,
        tomUpperCase	= 1,
        tomTitleCase	= 2,
        tomSentenceCase	= 4,
        tomToggleCase	= 5,
        tomReadOnly	= 0x100,
        tomShareDenyRead	= 0x200,
        tomShareDenyWrite	= 0x400,
        tomPasteFile	= 0x1000,
        tomCreateNew	= 0x10,
        tomCreateAlways	= 0x20,
        tomOpenExisting	= 0x30,
        tomOpenAlways	= 0x40,
        tomTruncateExisting	= 0x50,
        tomRTF	= 0x1,
        tomText	= 0x2,
        tomHTML	= 0x3,
        tomWordDocument	= 0x4,
        tomBold	= 0x80000001,
        tomItalic	= 0x80000002,
        tomUnderline	= 0x80000004,
        tomStrikeout	= 0x80000008,
        tomProtected	= 0x80000010,
        tomLink	= 0x80000020,
        tomSmallCaps	= 0x80000040,
        tomAllCaps	= 0x80000080,
        tomHidden	= 0x80000100,
        tomOutline	= 0x80000200,
        tomShadow	= 0x80000400,
        tomEmboss	= 0x80000800,
        tomImprint	= 0x80001000,
        tomDisabled	= 0x80002000,
        tomRevised	= 0x80004000,
        tomNormalCaret	= 0,
        tomKoreanBlockCaret	= 0x1,
        tomIncludeInset	= 0x1,
        tomIgnoreCurrentFont	= 0,
        tomMatchFontCharset	= 0x1,
        tomMatchFontSignature	= 0x2,
        tomCharset	= 0x80000000,
        tomRE10Mode	= 0x1,
        tomNoIME	= 0x80000,
        tomSelfIME	= 0x40000
    } 	tomConstants;







EXTERN_C const IID LIBID_tom;

#ifndef __ITextRange_INTERFACE_DEFINED__
#define __ITextRange_INTERFACE_DEFINED__

/* interface ITextRange */
/* [oleautomation][dual][object][uuid] */ 


EXTERN_C const IID IID_ITextRange;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8cc497c2-a1df-11ce-8098-00aa0047be5d")
    ITextRange : public IDispatch
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Text( 
            /* [out][retval] */ BSTR *text) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Text( 
            /* [in] */ BSTR text) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Char( 
            /* [out][retval] */ LONG *ch) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Char( 
            /* [in] */ LONG ch) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Duplicate( 
            /* [out][retval] */ ITextRange **range) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_FormattedText( 
            /* [out][retval] */ ITextRange **range) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_FormattedText( 
            /* [in] */ ITextRange *range) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Start( 
            /* [out][retval] */ LONG *start) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Start( 
            /* [in] */ LONG start) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_End( 
            /* [out][retval] */ LONG *end) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_End( 
            /* [in] */ LONG end) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Font( 
            /* [out][retval] */ ITextFont **font) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Font( 
            /* [in] */ ITextFont *font) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Para( 
            /* [out][retval] */ ITextPara **para) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Para( 
            /* [in] */ ITextPara *para) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_StoryLength( 
            /* [out][retval] */ LONG *len) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_StoryType( 
            /* [out][retval] */ LONG *value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Collapse( 
            /* [in] */ LONG start) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Expand( 
            /* [in] */ LONG unit,
            /* [out][retval] */ LONG *delta) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetIndex( 
            /* [in] */ LONG unit,
            /* [out][retval] */ LONG *index) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetIndex( 
            /* [in] */ LONG unit,
            /* [in] */ LONG index,
            /* [in] */ LONG extend) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetRange( 
            /* [in] */ LONG anchor,
            /* [in] */ LONG active) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE InRange( 
            /* [in] */ ITextRange *range,
            /* [out][retval] */ LONG *pb) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE InStory( 
            /* [in] */ ITextRange *range,
            /* [out][retval] */ LONG *pb) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE IsEqual( 
            /* [in] */ ITextRange *range,
            /* [out][retval] */ LONG *pb) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Select( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE StartOf( 
            /* [in] */ LONG unit,
            /* [in] */ LONG extend,
            /* [out][retval] */ LONG *delta) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EndOf( 
            /* [in] */ LONG unit,
            /* [in] */ LONG extend,
            /* [out][retval] */ LONG *delta) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Move( 
            /* [in] */ LONG unit,
            /* [in] */ LONG count,
            /* [out][retval] */ LONG *delta) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MoveStart( 
            /* [in] */ LONG unit,
            /* [in] */ LONG count,
            /* [out][retval] */ LONG *delta) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MoveEnd( 
            /* [in] */ LONG unit,
            /* [in] */ LONG count,
            /* [out][retval] */ LONG *delta) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MoveWhile( 
            /* [in] */ VARIANT *charset,
            /* [in] */ LONG count,
            /* [out][retval] */ LONG *delta) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MoveStartWhile( 
            /* [in] */ VARIANT *charset,
            /* [in] */ LONG count,
            /* [out][retval] */ LONG *delta) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MoveEndWhile( 
            /* [in] */ VARIANT *charset,
            /* [in] */ LONG count,
            /* [out][retval] */ LONG *delta) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MoveUntil( 
            /* [in] */ VARIANT *charset,
            /* [in] */ LONG count,
            /* [out][retval] */ LONG *delta) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MoveStartUntil( 
            /* [in] */ VARIANT *charset,
            /* [in] */ LONG count,
            /* [out][retval] */ LONG *delta) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MoveEndUntil( 
            /* [in] */ VARIANT *charset,
            /* [in] */ LONG count,
            /* [out][retval] */ LONG *delta) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FindText( 
            /* [in] */ BSTR str,
            /* [in] */ LONG ch,
            /* [in] */ LONG flags,
            /* [out][retval] */ LONG *len) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FindTextStart( 
            /* [in] */ BSTR str,
            /* [in] */ LONG cch,
            /* [in] */ LONG flags,
            /* [out][retval] */ LONG *len) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FindTextEnd( 
            /* [in] */ BSTR str,
            /* [in] */ LONG ch,
            /* [in] */ LONG flags,
            /* [out][retval] */ LONG *len) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Delete( 
            /* [in] */ LONG unit,
            /* [in] */ LONG count,
            /* [out][retval] */ LONG *delta) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Cut( 
            /* [out] */ VARIANT *var) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Copy( 
            /* [out] */ VARIANT *var) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Paste( 
            /* [in] */ VARIANT *var,
            /* [in] */ LONG format) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CanPaste( 
            /* [in] */ VARIANT *var,
            /* [in] */ LONG format,
            /* [out][retval] */ LONG *pb) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CanEdit( 
            /* [out][retval] */ LONG *pb) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ChangeCase( 
            /* [in] */ LONG type) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetPoint( 
            /* [in] */ LONG type,
            /* [out] */ LONG *cx,
            /* [out] */ LONG *cy) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetPoint( 
            /* [in] */ LONG x,
            /* [in] */ LONG y,
            /* [in] */ LONG type,
            /* [in] */ LONG extend) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ScrollIntoView( 
            /* [in] */ LONG value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetEmbeddedObject( 
            /* [out][retval] */ IUnknown **ppv) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ITextRangeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITextRange * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITextRange * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITextRange * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ITextRange * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ITextRange * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ITextRange * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ITextRange * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Text )( 
            ITextRange * This,
            /* [out][retval] */ BSTR *text);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Text )( 
            ITextRange * This,
            /* [in] */ BSTR text);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Char )( 
            ITextRange * This,
            /* [out][retval] */ LONG *ch);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_Char )( 
            ITextRange * This,
            /* [in] */ LONG ch);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Duplicate )( 
            ITextRange * This,
            /* [out][retval] */ ITextRange **range);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_FormattedText )( 
            ITextRange * This,
            /* [out][retval] */ ITextRange **range);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_FormattedText )( 
            ITextRange * This,
            /* [in] */ ITextRange *range);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Start )( 
            ITextRange * This,
            /* [out][retval] */ LONG *start);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_Start )( 
            ITextRange * This,
            /* [in] */ LONG start);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_End )( 
            ITextRange * This,
            /* [out][retval] */ LONG *end);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_End )( 
            ITextRange * This,
            /* [in] */ LONG end);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Font )( 
            ITextRange * This,
            /* [out][retval] */ ITextFont **font);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_Font )( 
            ITextRange * This,
            /* [in] */ ITextFont *font);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Para )( 
            ITextRange * This,
            /* [out][retval] */ ITextPara **para);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_Para )( 
            ITextRange * This,
            /* [in] */ ITextPara *para);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_StoryLength )( 
            ITextRange * This,
            /* [out][retval] */ LONG *len);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_StoryType )( 
            ITextRange * This,
            /* [out][retval] */ LONG *value);
        
        HRESULT ( STDMETHODCALLTYPE *Collapse )( 
            ITextRange * This,
            /* [in] */ LONG start);
        
        HRESULT ( STDMETHODCALLTYPE *Expand )( 
            ITextRange * This,
            /* [in] */ LONG unit,
            /* [out][retval] */ LONG *delta);
        
        HRESULT ( STDMETHODCALLTYPE *GetIndex )( 
            ITextRange * This,
            /* [in] */ LONG unit,
            /* [out][retval] */ LONG *index);
        
        HRESULT ( STDMETHODCALLTYPE *SetIndex )( 
            ITextRange * This,
            /* [in] */ LONG unit,
            /* [in] */ LONG index,
            /* [in] */ LONG extend);
        
        HRESULT ( STDMETHODCALLTYPE *SetRange )( 
            ITextRange * This,
            /* [in] */ LONG anchor,
            /* [in] */ LONG active);
        
        HRESULT ( STDMETHODCALLTYPE *InRange )( 
            ITextRange * This,
            /* [in] */ ITextRange *range,
            /* [out][retval] */ LONG *pb);
        
        HRESULT ( STDMETHODCALLTYPE *InStory )( 
            ITextRange * This,
            /* [in] */ ITextRange *range,
            /* [out][retval] */ LONG *pb);
        
        HRESULT ( STDMETHODCALLTYPE *IsEqual )( 
            ITextRange * This,
            /* [in] */ ITextRange *range,
            /* [out][retval] */ LONG *pb);
        
        HRESULT ( STDMETHODCALLTYPE *Select )( 
            ITextRange * This);
        
        HRESULT ( STDMETHODCALLTYPE *StartOf )( 
            ITextRange * This,
            /* [in] */ LONG unit,
            /* [in] */ LONG extend,
            /* [out][retval] */ LONG *delta);
        
        HRESULT ( STDMETHODCALLTYPE *EndOf )( 
            ITextRange * This,
            /* [in] */ LONG unit,
            /* [in] */ LONG extend,
            /* [out][retval] */ LONG *delta);
        
        HRESULT ( STDMETHODCALLTYPE *Move )( 
            ITextRange * This,
            /* [in] */ LONG unit,
            /* [in] */ LONG count,
            /* [out][retval] */ LONG *delta);
        
        HRESULT ( STDMETHODCALLTYPE *MoveStart )( 
            ITextRange * This,
            /* [in] */ LONG unit,
            /* [in] */ LONG count,
            /* [out][retval] */ LONG *delta);
        
        HRESULT ( STDMETHODCALLTYPE *MoveEnd )( 
            ITextRange * This,
            /* [in] */ LONG unit,
            /* [in] */ LONG count,
            /* [out][retval] */ LONG *delta);
        
        HRESULT ( STDMETHODCALLTYPE *MoveWhile )( 
            ITextRange * This,
            /* [in] */ VARIANT *charset,
            /* [in] */ LONG count,
            /* [out][retval] */ LONG *delta);
        
        HRESULT ( STDMETHODCALLTYPE *MoveStartWhile )( 
            ITextRange * This,
            /* [in] */ VARIANT *charset,
            /* [in] */ LONG count,
            /* [out][retval] */ LONG *delta);
        
        HRESULT ( STDMETHODCALLTYPE *MoveEndWhile )( 
            ITextRange * This,
            /* [in] */ VARIANT *charset,
            /* [in] */ LONG count,
            /* [out][retval] */ LONG *delta);
        
        HRESULT ( STDMETHODCALLTYPE *MoveUntil )( 
            ITextRange * This,
            /* [in] */ VARIANT *charset,
            /* [in] */ LONG count,
            /* [out][retval] */ LONG *delta);
        
        HRESULT ( STDMETHODCALLTYPE *MoveStartUntil )( 
            ITextRange * This,
            /* [in] */ VARIANT *charset,
            /* [in] */ LONG count,
            /* [out][retval] */ LONG *delta);
        
        HRESULT ( STDMETHODCALLTYPE *MoveEndUntil )( 
            ITextRange * This,
            /* [in] */ VARIANT *charset,
            /* [in] */ LONG count,
            /* [out][retval] */ LONG *delta);
        
        HRESULT ( STDMETHODCALLTYPE *FindText )( 
            ITextRange * This,
            /* [in] */ BSTR str,
            /* [in] */ LONG ch,
            /* [in] */ LONG flags,
            /* [out][retval] */ LONG *len);
        
        HRESULT ( STDMETHODCALLTYPE *FindTextStart )( 
            ITextRange * This,
            /* [in] */ BSTR str,
            /* [in] */ LONG cch,
            /* [in] */ LONG flags,
            /* [out][retval] */ LONG *len);
        
        HRESULT ( STDMETHODCALLTYPE *FindTextEnd )( 
            ITextRange * This,
            /* [in] */ BSTR str,
            /* [in] */ LONG ch,
            /* [in] */ LONG flags,
            /* [out][retval] */ LONG *len);
        
        HRESULT ( STDMETHODCALLTYPE *Delete )( 
            ITextRange * This,
            /* [in] */ LONG unit,
            /* [in] */ LONG count,
            /* [out][retval] */ LONG *delta);
        
        HRESULT ( STDMETHODCALLTYPE *Cut )( 
            ITextRange * This,
            /* [out] */ VARIANT *var);
        
        HRESULT ( STDMETHODCALLTYPE *Copy )( 
            ITextRange * This,
            /* [out] */ VARIANT *var);
        
        HRESULT ( STDMETHODCALLTYPE *Paste )( 
            ITextRange * This,
            /* [in] */ VARIANT *var,
            /* [in] */ LONG format);
        
        HRESULT ( STDMETHODCALLTYPE *CanPaste )( 
            ITextRange * This,
            /* [in] */ VARIANT *var,
            /* [in] */ LONG format,
            /* [out][retval] */ LONG *pb);
        
        HRESULT ( STDMETHODCALLTYPE *CanEdit )( 
            ITextRange * This,
            /* [out][retval] */ LONG *pb);
        
        HRESULT ( STDMETHODCALLTYPE *ChangeCase )( 
            ITextRange * This,
            /* [in] */ LONG type);
        
        HRESULT ( STDMETHODCALLTYPE *GetPoint )( 
            ITextRange * This,
            /* [in] */ LONG type,
            /* [out] */ LONG *cx,
            /* [out] */ LONG *cy);
        
        HRESULT ( STDMETHODCALLTYPE *SetPoint )( 
            ITextRange * This,
            /* [in] */ LONG x,
            /* [in] */ LONG y,
            /* [in] */ LONG type,
            /* [in] */ LONG extend);
        
        HRESULT ( STDMETHODCALLTYPE *ScrollIntoView )( 
            ITextRange * This,
            /* [in] */ LONG value);
        
        HRESULT ( STDMETHODCALLTYPE *GetEmbeddedObject )( 
            ITextRange * This,
            /* [out][retval] */ IUnknown **ppv);
        
        END_INTERFACE
    } ITextRangeVtbl;

    interface ITextRange
    {
        CONST_VTBL struct ITextRangeVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITextRange_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ITextRange_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ITextRange_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ITextRange_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ITextRange_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ITextRange_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ITextRange_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ITextRange_get_Text(This,text)	\
    ( (This)->lpVtbl -> get_Text(This,text) ) 

#define ITextRange_put_Text(This,text)	\
    ( (This)->lpVtbl -> put_Text(This,text) ) 

#define ITextRange_get_Char(This,ch)	\
    ( (This)->lpVtbl -> get_Char(This,ch) ) 

#define ITextRange_put_Char(This,ch)	\
    ( (This)->lpVtbl -> put_Char(This,ch) ) 

#define ITextRange_get_Duplicate(This,range)	\
    ( (This)->lpVtbl -> get_Duplicate(This,range) ) 

#define ITextRange_get_FormattedText(This,range)	\
    ( (This)->lpVtbl -> get_FormattedText(This,range) ) 

#define ITextRange_put_FormattedText(This,range)	\
    ( (This)->lpVtbl -> put_FormattedText(This,range) ) 

#define ITextRange_get_Start(This,start)	\
    ( (This)->lpVtbl -> get_Start(This,start) ) 

#define ITextRange_put_Start(This,start)	\
    ( (This)->lpVtbl -> put_Start(This,start) ) 

#define ITextRange_get_End(This,end)	\
    ( (This)->lpVtbl -> get_End(This,end) ) 

#define ITextRange_put_End(This,end)	\
    ( (This)->lpVtbl -> put_End(This,end) ) 

#define ITextRange_get_Font(This,font)	\
    ( (This)->lpVtbl -> get_Font(This,font) ) 

#define ITextRange_put_Font(This,font)	\
    ( (This)->lpVtbl -> put_Font(This,font) ) 

#define ITextRange_get_Para(This,para)	\
    ( (This)->lpVtbl -> get_Para(This,para) ) 

#define ITextRange_put_Para(This,para)	\
    ( (This)->lpVtbl -> put_Para(This,para) ) 

#define ITextRange_get_StoryLength(This,len)	\
    ( (This)->lpVtbl -> get_StoryLength(This,len) ) 

#define ITextRange_get_StoryType(This,value)	\
    ( (This)->lpVtbl -> get_StoryType(This,value) ) 

#define ITextRange_Collapse(This,start)	\
    ( (This)->lpVtbl -> Collapse(This,start) ) 

#define ITextRange_Expand(This,unit,delta)	\
    ( (This)->lpVtbl -> Expand(This,unit,delta) ) 

#define ITextRange_GetIndex(This,unit,index)	\
    ( (This)->lpVtbl -> GetIndex(This,unit,index) ) 

#define ITextRange_SetIndex(This,unit,index,extend)	\
    ( (This)->lpVtbl -> SetIndex(This,unit,index,extend) ) 

#define ITextRange_SetRange(This,anchor,active)	\
    ( (This)->lpVtbl -> SetRange(This,anchor,active) ) 

#define ITextRange_InRange(This,range,pb)	\
    ( (This)->lpVtbl -> InRange(This,range,pb) ) 

#define ITextRange_InStory(This,range,pb)	\
    ( (This)->lpVtbl -> InStory(This,range,pb) ) 

#define ITextRange_IsEqual(This,range,pb)	\
    ( (This)->lpVtbl -> IsEqual(This,range,pb) ) 

#define ITextRange_Select(This)	\
    ( (This)->lpVtbl -> Select(This) ) 

#define ITextRange_StartOf(This,unit,extend,delta)	\
    ( (This)->lpVtbl -> StartOf(This,unit,extend,delta) ) 

#define ITextRange_EndOf(This,unit,extend,delta)	\
    ( (This)->lpVtbl -> EndOf(This,unit,extend,delta) ) 

#define ITextRange_Move(This,unit,count,delta)	\
    ( (This)->lpVtbl -> Move(This,unit,count,delta) ) 

#define ITextRange_MoveStart(This,unit,count,delta)	\
    ( (This)->lpVtbl -> MoveStart(This,unit,count,delta) ) 

#define ITextRange_MoveEnd(This,unit,count,delta)	\
    ( (This)->lpVtbl -> MoveEnd(This,unit,count,delta) ) 

#define ITextRange_MoveWhile(This,charset,count,delta)	\
    ( (This)->lpVtbl -> MoveWhile(This,charset,count,delta) ) 

#define ITextRange_MoveStartWhile(This,charset,count,delta)	\
    ( (This)->lpVtbl -> MoveStartWhile(This,charset,count,delta) ) 

#define ITextRange_MoveEndWhile(This,charset,count,delta)	\
    ( (This)->lpVtbl -> MoveEndWhile(This,charset,count,delta) ) 

#define ITextRange_MoveUntil(This,charset,count,delta)	\
    ( (This)->lpVtbl -> MoveUntil(This,charset,count,delta) ) 

#define ITextRange_MoveStartUntil(This,charset,count,delta)	\
    ( (This)->lpVtbl -> MoveStartUntil(This,charset,count,delta) ) 

#define ITextRange_MoveEndUntil(This,charset,count,delta)	\
    ( (This)->lpVtbl -> MoveEndUntil(This,charset,count,delta) ) 

#define ITextRange_FindText(This,str,ch,flags,len)	\
    ( (This)->lpVtbl -> FindText(This,str,ch,flags,len) ) 

#define ITextRange_FindTextStart(This,str,cch,flags,len)	\
    ( (This)->lpVtbl -> FindTextStart(This,str,cch,flags,len) ) 

#define ITextRange_FindTextEnd(This,str,ch,flags,len)	\
    ( (This)->lpVtbl -> FindTextEnd(This,str,ch,flags,len) ) 

#define ITextRange_Delete(This,unit,count,delta)	\
    ( (This)->lpVtbl -> Delete(This,unit,count,delta) ) 

#define ITextRange_Cut(This,var)	\
    ( (This)->lpVtbl -> Cut(This,var) ) 

#define ITextRange_Copy(This,var)	\
    ( (This)->lpVtbl -> Copy(This,var) ) 

#define ITextRange_Paste(This,var,format)	\
    ( (This)->lpVtbl -> Paste(This,var,format) ) 

#define ITextRange_CanPaste(This,var,format,pb)	\
    ( (This)->lpVtbl -> CanPaste(This,var,format,pb) ) 

#define ITextRange_CanEdit(This,pb)	\
    ( (This)->lpVtbl -> CanEdit(This,pb) ) 

#define ITextRange_ChangeCase(This,type)	\
    ( (This)->lpVtbl -> ChangeCase(This,type) ) 

#define ITextRange_GetPoint(This,type,cx,cy)	\
    ( (This)->lpVtbl -> GetPoint(This,type,cx,cy) ) 

#define ITextRange_SetPoint(This,x,y,type,extend)	\
    ( (This)->lpVtbl -> SetPoint(This,x,y,type,extend) ) 

#define ITextRange_ScrollIntoView(This,value)	\
    ( (This)->lpVtbl -> ScrollIntoView(This,value) ) 

#define ITextRange_GetEmbeddedObject(This,ppv)	\
    ( (This)->lpVtbl -> GetEmbeddedObject(This,ppv) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ITextRange_INTERFACE_DEFINED__ */


#ifndef __ITextSelection_INTERFACE_DEFINED__
#define __ITextSelection_INTERFACE_DEFINED__

/* interface ITextSelection */
/* [oleautomation][dual][object][uuid] */ 


EXTERN_C const IID IID_ITextSelection;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8cc497c1-a1df-11ce-8098-00aa0047be5d")
    ITextSelection : public ITextRange
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Flags( 
            /* [out][retval] */ LONG *flags) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Flags( 
            /* [in] */ LONG flags) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Type( 
            /* [out][retval] */ LONG *type) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MoveLeft( 
            /* [in] */ LONG unit,
            /* [in] */ LONG count,
            /* [in] */ LONG extend,
            /* [out][retval] */ LONG *delta) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MoveRight( 
            /* [in] */ LONG unit,
            /* [in] */ LONG count,
            /* [in] */ LONG extend,
            /* [out][retval] */ LONG *delta) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MoveUp( 
            /* [in] */ LONG unit,
            /* [in] */ LONG count,
            /* [in] */ LONG extend,
            /* [out][retval] */ LONG *delta) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MoveDown( 
            /* [in] */ LONG unit,
            /* [in] */ LONG count,
            /* [in] */ LONG extend,
            /* [out][retval] */ LONG *delta) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE HomeKey( 
            /* [in] */ LONG unit,
            /* [in] */ LONG extend,
            /* [out][retval] */ LONG *delta) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EndKey( 
            /* [in] */ LONG unit,
            /* [in] */ LONG extend,
            /* [out][retval] */ LONG *delta) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE TypeText( 
            /* [in] */ BSTR str) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ITextSelectionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITextSelection * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITextSelection * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITextSelection * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ITextSelection * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ITextSelection * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ITextSelection * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ITextSelection * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Text )( 
            ITextSelection * This,
            /* [out][retval] */ BSTR *text);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Text )( 
            ITextSelection * This,
            /* [in] */ BSTR text);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Char )( 
            ITextSelection * This,
            /* [out][retval] */ LONG *ch);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_Char )( 
            ITextSelection * This,
            /* [in] */ LONG ch);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Duplicate )( 
            ITextSelection * This,
            /* [out][retval] */ ITextRange **range);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_FormattedText )( 
            ITextSelection * This,
            /* [out][retval] */ ITextRange **range);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_FormattedText )( 
            ITextSelection * This,
            /* [in] */ ITextRange *range);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Start )( 
            ITextSelection * This,
            /* [out][retval] */ LONG *start);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_Start )( 
            ITextSelection * This,
            /* [in] */ LONG start);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_End )( 
            ITextSelection * This,
            /* [out][retval] */ LONG *end);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_End )( 
            ITextSelection * This,
            /* [in] */ LONG end);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Font )( 
            ITextSelection * This,
            /* [out][retval] */ ITextFont **font);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_Font )( 
            ITextSelection * This,
            /* [in] */ ITextFont *font);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Para )( 
            ITextSelection * This,
            /* [out][retval] */ ITextPara **para);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_Para )( 
            ITextSelection * This,
            /* [in] */ ITextPara *para);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_StoryLength )( 
            ITextSelection * This,
            /* [out][retval] */ LONG *len);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_StoryType )( 
            ITextSelection * This,
            /* [out][retval] */ LONG *value);
        
        HRESULT ( STDMETHODCALLTYPE *Collapse )( 
            ITextSelection * This,
            /* [in] */ LONG start);
        
        HRESULT ( STDMETHODCALLTYPE *Expand )( 
            ITextSelection * This,
            /* [in] */ LONG unit,
            /* [out][retval] */ LONG *delta);
        
        HRESULT ( STDMETHODCALLTYPE *GetIndex )( 
            ITextSelection * This,
            /* [in] */ LONG unit,
            /* [out][retval] */ LONG *index);
        
        HRESULT ( STDMETHODCALLTYPE *SetIndex )( 
            ITextSelection * This,
            /* [in] */ LONG unit,
            /* [in] */ LONG index,
            /* [in] */ LONG extend);
        
        HRESULT ( STDMETHODCALLTYPE *SetRange )( 
            ITextSelection * This,
            /* [in] */ LONG anchor,
            /* [in] */ LONG active);
        
        HRESULT ( STDMETHODCALLTYPE *InRange )( 
            ITextSelection * This,
            /* [in] */ ITextRange *range,
            /* [out][retval] */ LONG *pb);
        
        HRESULT ( STDMETHODCALLTYPE *InStory )( 
            ITextSelection * This,
            /* [in] */ ITextRange *range,
            /* [out][retval] */ LONG *pb);
        
        HRESULT ( STDMETHODCALLTYPE *IsEqual )( 
            ITextSelection * This,
            /* [in] */ ITextRange *range,
            /* [out][retval] */ LONG *pb);
        
        HRESULT ( STDMETHODCALLTYPE *Select )( 
            ITextSelection * This);
        
        HRESULT ( STDMETHODCALLTYPE *StartOf )( 
            ITextSelection * This,
            /* [in] */ LONG unit,
            /* [in] */ LONG extend,
            /* [out][retval] */ LONG *delta);
        
        HRESULT ( STDMETHODCALLTYPE *EndOf )( 
            ITextSelection * This,
            /* [in] */ LONG unit,
            /* [in] */ LONG extend,
            /* [out][retval] */ LONG *delta);
        
        HRESULT ( STDMETHODCALLTYPE *Move )( 
            ITextSelection * This,
            /* [in] */ LONG unit,
            /* [in] */ LONG count,
            /* [out][retval] */ LONG *delta);
        
        HRESULT ( STDMETHODCALLTYPE *MoveStart )( 
            ITextSelection * This,
            /* [in] */ LONG unit,
            /* [in] */ LONG count,
            /* [out][retval] */ LONG *delta);
        
        HRESULT ( STDMETHODCALLTYPE *MoveEnd )( 
            ITextSelection * This,
            /* [in] */ LONG unit,
            /* [in] */ LONG count,
            /* [out][retval] */ LONG *delta);
        
        HRESULT ( STDMETHODCALLTYPE *MoveWhile )( 
            ITextSelection * This,
            /* [in] */ VARIANT *charset,
            /* [in] */ LONG count,
            /* [out][retval] */ LONG *delta);
        
        HRESULT ( STDMETHODCALLTYPE *MoveStartWhile )( 
            ITextSelection * This,
            /* [in] */ VARIANT *charset,
            /* [in] */ LONG count,
            /* [out][retval] */ LONG *delta);
        
        HRESULT ( STDMETHODCALLTYPE *MoveEndWhile )( 
            ITextSelection * This,
            /* [in] */ VARIANT *charset,
            /* [in] */ LONG count,
            /* [out][retval] */ LONG *delta);
        
        HRESULT ( STDMETHODCALLTYPE *MoveUntil )( 
            ITextSelection * This,
            /* [in] */ VARIANT *charset,
            /* [in] */ LONG count,
            /* [out][retval] */ LONG *delta);
        
        HRESULT ( STDMETHODCALLTYPE *MoveStartUntil )( 
            ITextSelection * This,
            /* [in] */ VARIANT *charset,
            /* [in] */ LONG count,
            /* [out][retval] */ LONG *delta);
        
        HRESULT ( STDMETHODCALLTYPE *MoveEndUntil )( 
            ITextSelection * This,
            /* [in] */ VARIANT *charset,
            /* [in] */ LONG count,
            /* [out][retval] */ LONG *delta);
        
        HRESULT ( STDMETHODCALLTYPE *FindText )( 
            ITextSelection * This,
            /* [in] */ BSTR str,
            /* [in] */ LONG ch,
            /* [in] */ LONG flags,
            /* [out][retval] */ LONG *len);
        
        HRESULT ( STDMETHODCALLTYPE *FindTextStart )( 
            ITextSelection * This,
            /* [in] */ BSTR str,
            /* [in] */ LONG cch,
            /* [in] */ LONG flags,
            /* [out][retval] */ LONG *len);
        
        HRESULT ( STDMETHODCALLTYPE *FindTextEnd )( 
            ITextSelection * This,
            /* [in] */ BSTR str,
            /* [in] */ LONG ch,
            /* [in] */ LONG flags,
            /* [out][retval] */ LONG *len);
        
        HRESULT ( STDMETHODCALLTYPE *Delete )( 
            ITextSelection * This,
            /* [in] */ LONG unit,
            /* [in] */ LONG count,
            /* [out][retval] */ LONG *delta);
        
        HRESULT ( STDMETHODCALLTYPE *Cut )( 
            ITextSelection * This,
            /* [out] */ VARIANT *var);
        
        HRESULT ( STDMETHODCALLTYPE *Copy )( 
            ITextSelection * This,
            /* [out] */ VARIANT *var);
        
        HRESULT ( STDMETHODCALLTYPE *Paste )( 
            ITextSelection * This,
            /* [in] */ VARIANT *var,
            /* [in] */ LONG format);
        
        HRESULT ( STDMETHODCALLTYPE *CanPaste )( 
            ITextSelection * This,
            /* [in] */ VARIANT *var,
            /* [in] */ LONG format,
            /* [out][retval] */ LONG *pb);
        
        HRESULT ( STDMETHODCALLTYPE *CanEdit )( 
            ITextSelection * This,
            /* [out][retval] */ LONG *pb);
        
        HRESULT ( STDMETHODCALLTYPE *ChangeCase )( 
            ITextSelection * This,
            /* [in] */ LONG type);
        
        HRESULT ( STDMETHODCALLTYPE *GetPoint )( 
            ITextSelection * This,
            /* [in] */ LONG type,
            /* [out] */ LONG *cx,
            /* [out] */ LONG *cy);
        
        HRESULT ( STDMETHODCALLTYPE *SetPoint )( 
            ITextSelection * This,
            /* [in] */ LONG x,
            /* [in] */ LONG y,
            /* [in] */ LONG type,
            /* [in] */ LONG extend);
        
        HRESULT ( STDMETHODCALLTYPE *ScrollIntoView )( 
            ITextSelection * This,
            /* [in] */ LONG value);
        
        HRESULT ( STDMETHODCALLTYPE *GetEmbeddedObject )( 
            ITextSelection * This,
            /* [out][retval] */ IUnknown **ppv);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Flags )( 
            ITextSelection * This,
            /* [out][retval] */ LONG *flags);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_Flags )( 
            ITextSelection * This,
            /* [in] */ LONG flags);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Type )( 
            ITextSelection * This,
            /* [out][retval] */ LONG *type);
        
        HRESULT ( STDMETHODCALLTYPE *MoveLeft )( 
            ITextSelection * This,
            /* [in] */ LONG unit,
            /* [in] */ LONG count,
            /* [in] */ LONG extend,
            /* [out][retval] */ LONG *delta);
        
        HRESULT ( STDMETHODCALLTYPE *MoveRight )( 
            ITextSelection * This,
            /* [in] */ LONG unit,
            /* [in] */ LONG count,
            /* [in] */ LONG extend,
            /* [out][retval] */ LONG *delta);
        
        HRESULT ( STDMETHODCALLTYPE *MoveUp )( 
            ITextSelection * This,
            /* [in] */ LONG unit,
            /* [in] */ LONG count,
            /* [in] */ LONG extend,
            /* [out][retval] */ LONG *delta);
        
        HRESULT ( STDMETHODCALLTYPE *MoveDown )( 
            ITextSelection * This,
            /* [in] */ LONG unit,
            /* [in] */ LONG count,
            /* [in] */ LONG extend,
            /* [out][retval] */ LONG *delta);
        
        HRESULT ( STDMETHODCALLTYPE *HomeKey )( 
            ITextSelection * This,
            /* [in] */ LONG unit,
            /* [in] */ LONG extend,
            /* [out][retval] */ LONG *delta);
        
        HRESULT ( STDMETHODCALLTYPE *EndKey )( 
            ITextSelection * This,
            /* [in] */ LONG unit,
            /* [in] */ LONG extend,
            /* [out][retval] */ LONG *delta);
        
        HRESULT ( STDMETHODCALLTYPE *TypeText )( 
            ITextSelection * This,
            /* [in] */ BSTR str);
        
        END_INTERFACE
    } ITextSelectionVtbl;

    interface ITextSelection
    {
        CONST_VTBL struct ITextSelectionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITextSelection_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ITextSelection_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ITextSelection_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ITextSelection_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ITextSelection_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ITextSelection_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ITextSelection_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ITextSelection_get_Text(This,text)	\
    ( (This)->lpVtbl -> get_Text(This,text) ) 

#define ITextSelection_put_Text(This,text)	\
    ( (This)->lpVtbl -> put_Text(This,text) ) 

#define ITextSelection_get_Char(This,ch)	\
    ( (This)->lpVtbl -> get_Char(This,ch) ) 

#define ITextSelection_put_Char(This,ch)	\
    ( (This)->lpVtbl -> put_Char(This,ch) ) 

#define ITextSelection_get_Duplicate(This,range)	\
    ( (This)->lpVtbl -> get_Duplicate(This,range) ) 

#define ITextSelection_get_FormattedText(This,range)	\
    ( (This)->lpVtbl -> get_FormattedText(This,range) ) 

#define ITextSelection_put_FormattedText(This,range)	\
    ( (This)->lpVtbl -> put_FormattedText(This,range) ) 

#define ITextSelection_get_Start(This,start)	\
    ( (This)->lpVtbl -> get_Start(This,start) ) 

#define ITextSelection_put_Start(This,start)	\
    ( (This)->lpVtbl -> put_Start(This,start) ) 

#define ITextSelection_get_End(This,end)	\
    ( (This)->lpVtbl -> get_End(This,end) ) 

#define ITextSelection_put_End(This,end)	\
    ( (This)->lpVtbl -> put_End(This,end) ) 

#define ITextSelection_get_Font(This,font)	\
    ( (This)->lpVtbl -> get_Font(This,font) ) 

#define ITextSelection_put_Font(This,font)	\
    ( (This)->lpVtbl -> put_Font(This,font) ) 

#define ITextSelection_get_Para(This,para)	\
    ( (This)->lpVtbl -> get_Para(This,para) ) 

#define ITextSelection_put_Para(This,para)	\
    ( (This)->lpVtbl -> put_Para(This,para) ) 

#define ITextSelection_get_StoryLength(This,len)	\
    ( (This)->lpVtbl -> get_StoryLength(This,len) ) 

#define ITextSelection_get_StoryType(This,value)	\
    ( (This)->lpVtbl -> get_StoryType(This,value) ) 

#define ITextSelection_Collapse(This,start)	\
    ( (This)->lpVtbl -> Collapse(This,start) ) 

#define ITextSelection_Expand(This,unit,delta)	\
    ( (This)->lpVtbl -> Expand(This,unit,delta) ) 

#define ITextSelection_GetIndex(This,unit,index)	\
    ( (This)->lpVtbl -> GetIndex(This,unit,index) ) 

#define ITextSelection_SetIndex(This,unit,index,extend)	\
    ( (This)->lpVtbl -> SetIndex(This,unit,index,extend) ) 

#define ITextSelection_SetRange(This,anchor,active)	\
    ( (This)->lpVtbl -> SetRange(This,anchor,active) ) 

#define ITextSelection_InRange(This,range,pb)	\
    ( (This)->lpVtbl -> InRange(This,range,pb) ) 

#define ITextSelection_InStory(This,range,pb)	\
    ( (This)->lpVtbl -> InStory(This,range,pb) ) 

#define ITextSelection_IsEqual(This,range,pb)	\
    ( (This)->lpVtbl -> IsEqual(This,range,pb) ) 

#define ITextSelection_Select(This)	\
    ( (This)->lpVtbl -> Select(This) ) 

#define ITextSelection_StartOf(This,unit,extend,delta)	\
    ( (This)->lpVtbl -> StartOf(This,unit,extend,delta) ) 

#define ITextSelection_EndOf(This,unit,extend,delta)	\
    ( (This)->lpVtbl -> EndOf(This,unit,extend,delta) ) 

#define ITextSelection_Move(This,unit,count,delta)	\
    ( (This)->lpVtbl -> Move(This,unit,count,delta) ) 

#define ITextSelection_MoveStart(This,unit,count,delta)	\
    ( (This)->lpVtbl -> MoveStart(This,unit,count,delta) ) 

#define ITextSelection_MoveEnd(This,unit,count,delta)	\
    ( (This)->lpVtbl -> MoveEnd(This,unit,count,delta) ) 

#define ITextSelection_MoveWhile(This,charset,count,delta)	\
    ( (This)->lpVtbl -> MoveWhile(This,charset,count,delta) ) 

#define ITextSelection_MoveStartWhile(This,charset,count,delta)	\
    ( (This)->lpVtbl -> MoveStartWhile(This,charset,count,delta) ) 

#define ITextSelection_MoveEndWhile(This,charset,count,delta)	\
    ( (This)->lpVtbl -> MoveEndWhile(This,charset,count,delta) ) 

#define ITextSelection_MoveUntil(This,charset,count,delta)	\
    ( (This)->lpVtbl -> MoveUntil(This,charset,count,delta) ) 

#define ITextSelection_MoveStartUntil(This,charset,count,delta)	\
    ( (This)->lpVtbl -> MoveStartUntil(This,charset,count,delta) ) 

#define ITextSelection_MoveEndUntil(This,charset,count,delta)	\
    ( (This)->lpVtbl -> MoveEndUntil(This,charset,count,delta) ) 

#define ITextSelection_FindText(This,str,ch,flags,len)	\
    ( (This)->lpVtbl -> FindText(This,str,ch,flags,len) ) 

#define ITextSelection_FindTextStart(This,str,cch,flags,len)	\
    ( (This)->lpVtbl -> FindTextStart(This,str,cch,flags,len) ) 

#define ITextSelection_FindTextEnd(This,str,ch,flags,len)	\
    ( (This)->lpVtbl -> FindTextEnd(This,str,ch,flags,len) ) 

#define ITextSelection_Delete(This,unit,count,delta)	\
    ( (This)->lpVtbl -> Delete(This,unit,count,delta) ) 

#define ITextSelection_Cut(This,var)	\
    ( (This)->lpVtbl -> Cut(This,var) ) 

#define ITextSelection_Copy(This,var)	\
    ( (This)->lpVtbl -> Copy(This,var) ) 

#define ITextSelection_Paste(This,var,format)	\
    ( (This)->lpVtbl -> Paste(This,var,format) ) 

#define ITextSelection_CanPaste(This,var,format,pb)	\
    ( (This)->lpVtbl -> CanPaste(This,var,format,pb) ) 

#define ITextSelection_CanEdit(This,pb)	\
    ( (This)->lpVtbl -> CanEdit(This,pb) ) 

#define ITextSelection_ChangeCase(This,type)	\
    ( (This)->lpVtbl -> ChangeCase(This,type) ) 

#define ITextSelection_GetPoint(This,type,cx,cy)	\
    ( (This)->lpVtbl -> GetPoint(This,type,cx,cy) ) 

#define ITextSelection_SetPoint(This,x,y,type,extend)	\
    ( (This)->lpVtbl -> SetPoint(This,x,y,type,extend) ) 

#define ITextSelection_ScrollIntoView(This,value)	\
    ( (This)->lpVtbl -> ScrollIntoView(This,value) ) 

#define ITextSelection_GetEmbeddedObject(This,ppv)	\
    ( (This)->lpVtbl -> GetEmbeddedObject(This,ppv) ) 


#define ITextSelection_get_Flags(This,flags)	\
    ( (This)->lpVtbl -> get_Flags(This,flags) ) 

#define ITextSelection_put_Flags(This,flags)	\
    ( (This)->lpVtbl -> put_Flags(This,flags) ) 

#define ITextSelection_get_Type(This,type)	\
    ( (This)->lpVtbl -> get_Type(This,type) ) 

#define ITextSelection_MoveLeft(This,unit,count,extend,delta)	\
    ( (This)->lpVtbl -> MoveLeft(This,unit,count,extend,delta) ) 

#define ITextSelection_MoveRight(This,unit,count,extend,delta)	\
    ( (This)->lpVtbl -> MoveRight(This,unit,count,extend,delta) ) 

#define ITextSelection_MoveUp(This,unit,count,extend,delta)	\
    ( (This)->lpVtbl -> MoveUp(This,unit,count,extend,delta) ) 

#define ITextSelection_MoveDown(This,unit,count,extend,delta)	\
    ( (This)->lpVtbl -> MoveDown(This,unit,count,extend,delta) ) 

#define ITextSelection_HomeKey(This,unit,extend,delta)	\
    ( (This)->lpVtbl -> HomeKey(This,unit,extend,delta) ) 

#define ITextSelection_EndKey(This,unit,extend,delta)	\
    ( (This)->lpVtbl -> EndKey(This,unit,extend,delta) ) 

#define ITextSelection_TypeText(This,str)	\
    ( (This)->lpVtbl -> TypeText(This,str) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ITextSelection_INTERFACE_DEFINED__ */


#ifndef __ITextStoryRanges_INTERFACE_DEFINED__
#define __ITextStoryRanges_INTERFACE_DEFINED__

/* interface ITextStoryRanges */
/* [oleautomation][dual][object][uuid] */ 


EXTERN_C const IID IID_ITextStoryRanges;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8cc497c5-a1df-11ce-8098-00aa0047be5d")
    ITextStoryRanges : public IDispatch
    {
    public:
        virtual /* [id][hidden][restricted] */ HRESULT STDMETHODCALLTYPE _NewEnum( 
            /* [out][retval] */ IUnknown **ppEnum) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Item( 
            /* [in] */ LONG index,
            /* [out][retval] */ ITextRange **range) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [out][retval] */ LONG *count) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ITextStoryRangesVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITextStoryRanges * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITextStoryRanges * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITextStoryRanges * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ITextStoryRanges * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ITextStoryRanges * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ITextStoryRanges * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ITextStoryRanges * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [id][hidden][restricted] */ HRESULT ( STDMETHODCALLTYPE *_NewEnum )( 
            ITextStoryRanges * This,
            /* [out][retval] */ IUnknown **ppEnum);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Item )( 
            ITextStoryRanges * This,
            /* [in] */ LONG index,
            /* [out][retval] */ ITextRange **range);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            ITextStoryRanges * This,
            /* [out][retval] */ LONG *count);
        
        END_INTERFACE
    } ITextStoryRangesVtbl;

    interface ITextStoryRanges
    {
        CONST_VTBL struct ITextStoryRangesVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITextStoryRanges_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ITextStoryRanges_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ITextStoryRanges_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ITextStoryRanges_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ITextStoryRanges_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ITextStoryRanges_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ITextStoryRanges_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ITextStoryRanges__NewEnum(This,ppEnum)	\
    ( (This)->lpVtbl -> _NewEnum(This,ppEnum) ) 

#define ITextStoryRanges_Item(This,index,range)	\
    ( (This)->lpVtbl -> Item(This,index,range) ) 

#define ITextStoryRanges_get_Count(This,count)	\
    ( (This)->lpVtbl -> get_Count(This,count) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ITextStoryRanges_INTERFACE_DEFINED__ */


#ifndef __ITextDocument_INTERFACE_DEFINED__
#define __ITextDocument_INTERFACE_DEFINED__

/* interface ITextDocument */
/* [oleautomation][dual][object][uuid] */ 


EXTERN_C const IID IID_ITextDocument;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8cc497c0-a1df-11ce-8098-00aa0047be5d")
    ITextDocument : public IDispatch
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [out][retval] */ BSTR *name) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Selection( 
            /* [out][retval] */ ITextSelection **selection) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_StoryCount( 
            /* [out][retval] */ LONG *count) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_StoryRanges( 
            /* [out][retval] */ ITextStoryRanges **ranges) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Saved( 
            /* [out][retval] */ LONG *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Saved( 
            /* [in] */ LONG value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_DefaultTabStop( 
            /* [out][retval] */ float *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_DefaultTabStop( 
            /* [in] */ float value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE New( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Open( 
            /* [in] */ VARIANT *var,
            /* [in] */ LONG flags,
            /* [in] */ LONG codepage) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Save( 
            /* [in] */ VARIANT *var,
            /* [in] */ LONG flags,
            /* [in] */ LONG codepage) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Freeze( 
            /* [out][retval] */ LONG *count) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Unfreeze( 
            /* [out][retval] */ LONG *count) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE BeginEditCollection( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EndEditCollection( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Undo( 
            /* [in] */ LONG count,
            /* [out][retval] */ LONG *prop) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Redo( 
            /* [in] */ LONG count,
            /* [out][retval] */ LONG *prop) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Range( 
            /* [in] */ LONG start,
            /* [in] */ LONG end,
            /* [out][retval] */ ITextRange **range) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RangeFromPoint( 
            /* [in] */ LONG x,
            /* [in] */ LONG y,
            /* [out][retval] */ ITextRange **range) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ITextDocumentVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITextDocument * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITextDocument * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITextDocument * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ITextDocument * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ITextDocument * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ITextDocument * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ITextDocument * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            ITextDocument * This,
            /* [out][retval] */ BSTR *name);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Selection )( 
            ITextDocument * This,
            /* [out][retval] */ ITextSelection **selection);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_StoryCount )( 
            ITextDocument * This,
            /* [out][retval] */ LONG *count);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_StoryRanges )( 
            ITextDocument * This,
            /* [out][retval] */ ITextStoryRanges **ranges);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Saved )( 
            ITextDocument * This,
            /* [out][retval] */ LONG *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_Saved )( 
            ITextDocument * This,
            /* [in] */ LONG value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_DefaultTabStop )( 
            ITextDocument * This,
            /* [out][retval] */ float *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_DefaultTabStop )( 
            ITextDocument * This,
            /* [in] */ float value);
        
        HRESULT ( STDMETHODCALLTYPE *New )( 
            ITextDocument * This);
        
        HRESULT ( STDMETHODCALLTYPE *Open )( 
            ITextDocument * This,
            /* [in] */ VARIANT *var,
            /* [in] */ LONG flags,
            /* [in] */ LONG codepage);
        
        HRESULT ( STDMETHODCALLTYPE *Save )( 
            ITextDocument * This,
            /* [in] */ VARIANT *var,
            /* [in] */ LONG flags,
            /* [in] */ LONG codepage);
        
        HRESULT ( STDMETHODCALLTYPE *Freeze )( 
            ITextDocument * This,
            /* [out][retval] */ LONG *count);
        
        HRESULT ( STDMETHODCALLTYPE *Unfreeze )( 
            ITextDocument * This,
            /* [out][retval] */ LONG *count);
        
        HRESULT ( STDMETHODCALLTYPE *BeginEditCollection )( 
            ITextDocument * This);
        
        HRESULT ( STDMETHODCALLTYPE *EndEditCollection )( 
            ITextDocument * This);
        
        HRESULT ( STDMETHODCALLTYPE *Undo )( 
            ITextDocument * This,
            /* [in] */ LONG count,
            /* [out][retval] */ LONG *prop);
        
        HRESULT ( STDMETHODCALLTYPE *Redo )( 
            ITextDocument * This,
            /* [in] */ LONG count,
            /* [out][retval] */ LONG *prop);
        
        HRESULT ( STDMETHODCALLTYPE *Range )( 
            ITextDocument * This,
            /* [in] */ LONG start,
            /* [in] */ LONG end,
            /* [out][retval] */ ITextRange **range);
        
        HRESULT ( STDMETHODCALLTYPE *RangeFromPoint )( 
            ITextDocument * This,
            /* [in] */ LONG x,
            /* [in] */ LONG y,
            /* [out][retval] */ ITextRange **range);
        
        END_INTERFACE
    } ITextDocumentVtbl;

    interface ITextDocument
    {
        CONST_VTBL struct ITextDocumentVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITextDocument_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ITextDocument_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ITextDocument_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ITextDocument_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ITextDocument_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ITextDocument_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ITextDocument_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ITextDocument_get_Name(This,name)	\
    ( (This)->lpVtbl -> get_Name(This,name) ) 

#define ITextDocument_get_Selection(This,selection)	\
    ( (This)->lpVtbl -> get_Selection(This,selection) ) 

#define ITextDocument_get_StoryCount(This,count)	\
    ( (This)->lpVtbl -> get_StoryCount(This,count) ) 

#define ITextDocument_get_StoryRanges(This,ranges)	\
    ( (This)->lpVtbl -> get_StoryRanges(This,ranges) ) 

#define ITextDocument_get_Saved(This,value)	\
    ( (This)->lpVtbl -> get_Saved(This,value) ) 

#define ITextDocument_put_Saved(This,value)	\
    ( (This)->lpVtbl -> put_Saved(This,value) ) 

#define ITextDocument_get_DefaultTabStop(This,value)	\
    ( (This)->lpVtbl -> get_DefaultTabStop(This,value) ) 

#define ITextDocument_put_DefaultTabStop(This,value)	\
    ( (This)->lpVtbl -> put_DefaultTabStop(This,value) ) 

#define ITextDocument_New(This)	\
    ( (This)->lpVtbl -> New(This) ) 

#define ITextDocument_Open(This,var,flags,codepage)	\
    ( (This)->lpVtbl -> Open(This,var,flags,codepage) ) 

#define ITextDocument_Save(This,var,flags,codepage)	\
    ( (This)->lpVtbl -> Save(This,var,flags,codepage) ) 

#define ITextDocument_Freeze(This,count)	\
    ( (This)->lpVtbl -> Freeze(This,count) ) 

#define ITextDocument_Unfreeze(This,count)	\
    ( (This)->lpVtbl -> Unfreeze(This,count) ) 

#define ITextDocument_BeginEditCollection(This)	\
    ( (This)->lpVtbl -> BeginEditCollection(This) ) 

#define ITextDocument_EndEditCollection(This)	\
    ( (This)->lpVtbl -> EndEditCollection(This) ) 

#define ITextDocument_Undo(This,count,prop)	\
    ( (This)->lpVtbl -> Undo(This,count,prop) ) 

#define ITextDocument_Redo(This,count,prop)	\
    ( (This)->lpVtbl -> Redo(This,count,prop) ) 

#define ITextDocument_Range(This,start,end,range)	\
    ( (This)->lpVtbl -> Range(This,start,end,range) ) 

#define ITextDocument_RangeFromPoint(This,x,y,range)	\
    ( (This)->lpVtbl -> RangeFromPoint(This,x,y,range) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ITextDocument_INTERFACE_DEFINED__ */


#ifndef __ITextFont_INTERFACE_DEFINED__
#define __ITextFont_INTERFACE_DEFINED__

/* interface ITextFont */
/* [oleautomation][dual][object][uuid] */ 


EXTERN_C const IID IID_ITextFont;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8cc497c3-a1df-11ce-8098-00aa0047be5d")
    ITextFont : public IDispatch
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Duplicate( 
            /* [out][retval] */ ITextFont **font) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Duplicate( 
            /* [in] */ ITextFont *font) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CanChange( 
            /* [out] */ LONG *pB) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE IsEqual( 
            /* [in] */ ITextFont *font,
            /* [out][retval] */ LONG *pB) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Reset( 
            /* [in] */ LONG value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_GetStyle( 
            /* [out][retval] */ LONG *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_SetStyle( 
            /* [in] */ LONG value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_AllCaps( 
            /* [out][retval] */ LONG *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_AllCaps( 
            /* [in] */ LONG value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Animation( 
            /* [out][retval] */ LONG *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Animation( 
            /* [in] */ LONG value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_BackColor( 
            /* [out][retval] */ LONG *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_BackColor( 
            /* [in] */ LONG value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Bold( 
            /* [out][retval] */ LONG *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Bold( 
            /* [in] */ LONG value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Emboss( 
            /* [out][retval] */ LONG *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Emboss( 
            /* [in] */ LONG value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_ForeColor( 
            /* [out][retval] */ LONG *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_ForeColor( 
            /* [in] */ LONG value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Hidden( 
            /* [out][retval] */ LONG *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Hidden( 
            /* [in] */ LONG value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Engrave( 
            /* [out][retval] */ LONG *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Engrave( 
            /* [in] */ LONG value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Italic( 
            /* [out][retval] */ LONG *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Italic( 
            /* [in] */ LONG value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Kerning( 
            /* [out][retval] */ float *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Kerning( 
            /* [in] */ float value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_LanguageID( 
            /* [out][retval] */ LONG *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_LanguageID( 
            /* [in] */ LONG value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [out][retval] */ BSTR *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Name( 
            /* [in] */ BSTR value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Outline( 
            /* [out][retval] */ LONG *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Outline( 
            /* [in] */ LONG value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Position( 
            /* [out][retval] */ float *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Position( 
            /* [in] */ float value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Protected( 
            /* [out][retval] */ LONG *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Protected( 
            /* [in] */ LONG value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Shadow( 
            /* [out][retval] */ LONG *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Shadow( 
            /* [in] */ LONG value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Size( 
            /* [out][retval] */ float *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Size( 
            /* [in] */ float value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_SmallCaps( 
            /* [out][retval] */ LONG *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_SmallCaps( 
            /* [in] */ LONG value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Spacing( 
            /* [out][retval] */ float *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Spacing( 
            /* [in] */ float value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_StrikeThrough( 
            /* [out][retval] */ LONG *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_StrikeThrough( 
            /* [in] */ LONG value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Subscript( 
            /* [out][retval] */ LONG *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Subscript( 
            /* [in] */ LONG value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Superscript( 
            /* [out][retval] */ LONG *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Superscript( 
            /* [in] */ LONG value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Underline( 
            /* [out][retval] */ LONG *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Underline( 
            /* [in] */ LONG value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Weight( 
            /* [out][retval] */ LONG *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Weight( 
            /* [in] */ LONG value) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ITextFontVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITextFont * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITextFont * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITextFont * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ITextFont * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ITextFont * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ITextFont * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ITextFont * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Duplicate )( 
            ITextFont * This,
            /* [out][retval] */ ITextFont **font);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Duplicate )( 
            ITextFont * This,
            /* [in] */ ITextFont *font);
        
        HRESULT ( STDMETHODCALLTYPE *CanChange )( 
            ITextFont * This,
            /* [out] */ LONG *pB);
        
        HRESULT ( STDMETHODCALLTYPE *IsEqual )( 
            ITextFont * This,
            /* [in] */ ITextFont *font,
            /* [out][retval] */ LONG *pB);
        
        HRESULT ( STDMETHODCALLTYPE *Reset )( 
            ITextFont * This,
            /* [in] */ LONG value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_GetStyle )( 
            ITextFont * This,
            /* [out][retval] */ LONG *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_SetStyle )( 
            ITextFont * This,
            /* [in] */ LONG value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_AllCaps )( 
            ITextFont * This,
            /* [out][retval] */ LONG *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_AllCaps )( 
            ITextFont * This,
            /* [in] */ LONG value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Animation )( 
            ITextFont * This,
            /* [out][retval] */ LONG *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_Animation )( 
            ITextFont * This,
            /* [in] */ LONG value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_BackColor )( 
            ITextFont * This,
            /* [out][retval] */ LONG *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_BackColor )( 
            ITextFont * This,
            /* [in] */ LONG value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Bold )( 
            ITextFont * This,
            /* [out][retval] */ LONG *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_Bold )( 
            ITextFont * This,
            /* [in] */ LONG value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Emboss )( 
            ITextFont * This,
            /* [out][retval] */ LONG *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_Emboss )( 
            ITextFont * This,
            /* [in] */ LONG value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_ForeColor )( 
            ITextFont * This,
            /* [out][retval] */ LONG *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_ForeColor )( 
            ITextFont * This,
            /* [in] */ LONG value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Hidden )( 
            ITextFont * This,
            /* [out][retval] */ LONG *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_Hidden )( 
            ITextFont * This,
            /* [in] */ LONG value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Engrave )( 
            ITextFont * This,
            /* [out][retval] */ LONG *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_Engrave )( 
            ITextFont * This,
            /* [in] */ LONG value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Italic )( 
            ITextFont * This,
            /* [out][retval] */ LONG *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_Italic )( 
            ITextFont * This,
            /* [in] */ LONG value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Kerning )( 
            ITextFont * This,
            /* [out][retval] */ float *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_Kerning )( 
            ITextFont * This,
            /* [in] */ float value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_LanguageID )( 
            ITextFont * This,
            /* [out][retval] */ LONG *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_LanguageID )( 
            ITextFont * This,
            /* [in] */ LONG value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            ITextFont * This,
            /* [out][retval] */ BSTR *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_Name )( 
            ITextFont * This,
            /* [in] */ BSTR value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Outline )( 
            ITextFont * This,
            /* [out][retval] */ LONG *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_Outline )( 
            ITextFont * This,
            /* [in] */ LONG value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Position )( 
            ITextFont * This,
            /* [out][retval] */ float *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_Position )( 
            ITextFont * This,
            /* [in] */ float value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Protected )( 
            ITextFont * This,
            /* [out][retval] */ LONG *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_Protected )( 
            ITextFont * This,
            /* [in] */ LONG value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Shadow )( 
            ITextFont * This,
            /* [out][retval] */ LONG *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_Shadow )( 
            ITextFont * This,
            /* [in] */ LONG value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Size )( 
            ITextFont * This,
            /* [out][retval] */ float *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_Size )( 
            ITextFont * This,
            /* [in] */ float value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_SmallCaps )( 
            ITextFont * This,
            /* [out][retval] */ LONG *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_SmallCaps )( 
            ITextFont * This,
            /* [in] */ LONG value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Spacing )( 
            ITextFont * This,
            /* [out][retval] */ float *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_Spacing )( 
            ITextFont * This,
            /* [in] */ float value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_StrikeThrough )( 
            ITextFont * This,
            /* [out][retval] */ LONG *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_StrikeThrough )( 
            ITextFont * This,
            /* [in] */ LONG value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Subscript )( 
            ITextFont * This,
            /* [out][retval] */ LONG *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_Subscript )( 
            ITextFont * This,
            /* [in] */ LONG value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Superscript )( 
            ITextFont * This,
            /* [out][retval] */ LONG *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_Superscript )( 
            ITextFont * This,
            /* [in] */ LONG value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Underline )( 
            ITextFont * This,
            /* [out][retval] */ LONG *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_Underline )( 
            ITextFont * This,
            /* [in] */ LONG value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Weight )( 
            ITextFont * This,
            /* [out][retval] */ LONG *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_Weight )( 
            ITextFont * This,
            /* [in] */ LONG value);
        
        END_INTERFACE
    } ITextFontVtbl;

    interface ITextFont
    {
        CONST_VTBL struct ITextFontVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITextFont_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ITextFont_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ITextFont_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ITextFont_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ITextFont_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ITextFont_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ITextFont_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ITextFont_get_Duplicate(This,font)	\
    ( (This)->lpVtbl -> get_Duplicate(This,font) ) 

#define ITextFont_put_Duplicate(This,font)	\
    ( (This)->lpVtbl -> put_Duplicate(This,font) ) 

#define ITextFont_CanChange(This,pB)	\
    ( (This)->lpVtbl -> CanChange(This,pB) ) 

#define ITextFont_IsEqual(This,font,pB)	\
    ( (This)->lpVtbl -> IsEqual(This,font,pB) ) 

#define ITextFont_Reset(This,value)	\
    ( (This)->lpVtbl -> Reset(This,value) ) 

#define ITextFont_get_GetStyle(This,value)	\
    ( (This)->lpVtbl -> get_GetStyle(This,value) ) 

#define ITextFont_put_SetStyle(This,value)	\
    ( (This)->lpVtbl -> put_SetStyle(This,value) ) 

#define ITextFont_get_AllCaps(This,value)	\
    ( (This)->lpVtbl -> get_AllCaps(This,value) ) 

#define ITextFont_put_AllCaps(This,value)	\
    ( (This)->lpVtbl -> put_AllCaps(This,value) ) 

#define ITextFont_get_Animation(This,value)	\
    ( (This)->lpVtbl -> get_Animation(This,value) ) 

#define ITextFont_put_Animation(This,value)	\
    ( (This)->lpVtbl -> put_Animation(This,value) ) 

#define ITextFont_get_BackColor(This,value)	\
    ( (This)->lpVtbl -> get_BackColor(This,value) ) 

#define ITextFont_put_BackColor(This,value)	\
    ( (This)->lpVtbl -> put_BackColor(This,value) ) 

#define ITextFont_get_Bold(This,value)	\
    ( (This)->lpVtbl -> get_Bold(This,value) ) 

#define ITextFont_put_Bold(This,value)	\
    ( (This)->lpVtbl -> put_Bold(This,value) ) 

#define ITextFont_get_Emboss(This,value)	\
    ( (This)->lpVtbl -> get_Emboss(This,value) ) 

#define ITextFont_put_Emboss(This,value)	\
    ( (This)->lpVtbl -> put_Emboss(This,value) ) 

#define ITextFont_get_ForeColor(This,value)	\
    ( (This)->lpVtbl -> get_ForeColor(This,value) ) 

#define ITextFont_put_ForeColor(This,value)	\
    ( (This)->lpVtbl -> put_ForeColor(This,value) ) 

#define ITextFont_get_Hidden(This,value)	\
    ( (This)->lpVtbl -> get_Hidden(This,value) ) 

#define ITextFont_put_Hidden(This,value)	\
    ( (This)->lpVtbl -> put_Hidden(This,value) ) 

#define ITextFont_get_Engrave(This,value)	\
    ( (This)->lpVtbl -> get_Engrave(This,value) ) 

#define ITextFont_put_Engrave(This,value)	\
    ( (This)->lpVtbl -> put_Engrave(This,value) ) 

#define ITextFont_get_Italic(This,value)	\
    ( (This)->lpVtbl -> get_Italic(This,value) ) 

#define ITextFont_put_Italic(This,value)	\
    ( (This)->lpVtbl -> put_Italic(This,value) ) 

#define ITextFont_get_Kerning(This,value)	\
    ( (This)->lpVtbl -> get_Kerning(This,value) ) 

#define ITextFont_put_Kerning(This,value)	\
    ( (This)->lpVtbl -> put_Kerning(This,value) ) 

#define ITextFont_get_LanguageID(This,value)	\
    ( (This)->lpVtbl -> get_LanguageID(This,value) ) 

#define ITextFont_put_LanguageID(This,value)	\
    ( (This)->lpVtbl -> put_LanguageID(This,value) ) 

#define ITextFont_get_Name(This,value)	\
    ( (This)->lpVtbl -> get_Name(This,value) ) 

#define ITextFont_put_Name(This,value)	\
    ( (This)->lpVtbl -> put_Name(This,value) ) 

#define ITextFont_get_Outline(This,value)	\
    ( (This)->lpVtbl -> get_Outline(This,value) ) 

#define ITextFont_put_Outline(This,value)	\
    ( (This)->lpVtbl -> put_Outline(This,value) ) 

#define ITextFont_get_Position(This,value)	\
    ( (This)->lpVtbl -> get_Position(This,value) ) 

#define ITextFont_put_Position(This,value)	\
    ( (This)->lpVtbl -> put_Position(This,value) ) 

#define ITextFont_get_Protected(This,value)	\
    ( (This)->lpVtbl -> get_Protected(This,value) ) 

#define ITextFont_put_Protected(This,value)	\
    ( (This)->lpVtbl -> put_Protected(This,value) ) 

#define ITextFont_get_Shadow(This,value)	\
    ( (This)->lpVtbl -> get_Shadow(This,value) ) 

#define ITextFont_put_Shadow(This,value)	\
    ( (This)->lpVtbl -> put_Shadow(This,value) ) 

#define ITextFont_get_Size(This,value)	\
    ( (This)->lpVtbl -> get_Size(This,value) ) 

#define ITextFont_put_Size(This,value)	\
    ( (This)->lpVtbl -> put_Size(This,value) ) 

#define ITextFont_get_SmallCaps(This,value)	\
    ( (This)->lpVtbl -> get_SmallCaps(This,value) ) 

#define ITextFont_put_SmallCaps(This,value)	\
    ( (This)->lpVtbl -> put_SmallCaps(This,value) ) 

#define ITextFont_get_Spacing(This,value)	\
    ( (This)->lpVtbl -> get_Spacing(This,value) ) 

#define ITextFont_put_Spacing(This,value)	\
    ( (This)->lpVtbl -> put_Spacing(This,value) ) 

#define ITextFont_get_StrikeThrough(This,value)	\
    ( (This)->lpVtbl -> get_StrikeThrough(This,value) ) 

#define ITextFont_put_StrikeThrough(This,value)	\
    ( (This)->lpVtbl -> put_StrikeThrough(This,value) ) 

#define ITextFont_get_Subscript(This,value)	\
    ( (This)->lpVtbl -> get_Subscript(This,value) ) 

#define ITextFont_put_Subscript(This,value)	\
    ( (This)->lpVtbl -> put_Subscript(This,value) ) 

#define ITextFont_get_Superscript(This,value)	\
    ( (This)->lpVtbl -> get_Superscript(This,value) ) 

#define ITextFont_put_Superscript(This,value)	\
    ( (This)->lpVtbl -> put_Superscript(This,value) ) 

#define ITextFont_get_Underline(This,value)	\
    ( (This)->lpVtbl -> get_Underline(This,value) ) 

#define ITextFont_put_Underline(This,value)	\
    ( (This)->lpVtbl -> put_Underline(This,value) ) 

#define ITextFont_get_Weight(This,value)	\
    ( (This)->lpVtbl -> get_Weight(This,value) ) 

#define ITextFont_put_Weight(This,value)	\
    ( (This)->lpVtbl -> put_Weight(This,value) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ITextFont_INTERFACE_DEFINED__ */


#ifndef __ITextPara_INTERFACE_DEFINED__
#define __ITextPara_INTERFACE_DEFINED__

/* interface ITextPara */
/* [oleautomation][dual][object][uuid] */ 


EXTERN_C const IID IID_ITextPara;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8cc497c4-a1df-11ce-8098-00aa0047be5d")
    ITextPara : public IDispatch
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Duplicate( 
            /* [out][retval] */ ITextPara **para) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Duplicate( 
            /* [in] */ ITextPara *para) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CanChange( 
            /* [retval][out] */ LONG *pB) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE IsEqual( 
            /* [in] */ ITextPara *para,
            /* [out][retval] */ LONG *pB) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Reset( 
            /* [in] */ LONG value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Style( 
            /* [out][retval] */ LONG *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Style( 
            /* [in] */ LONG value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Alignment( 
            /* [out][retval] */ LONG *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Alignment( 
            /* [in] */ LONG value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Hyphenation( 
            /* [out][retval] */ LONG *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Hyphenation( 
            /* [in] */ LONG value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_FirstLineIndent( 
            /* [out][retval] */ float *value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_KeepTogether( 
            /* [out][retval] */ LONG *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_KeepTogether( 
            /* [in] */ LONG value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_KeepWithNext( 
            /* [out][retval] */ LONG *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_KeepWithNext( 
            /* [in] */ LONG value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_LeftIndent( 
            /* [out][retval] */ float *value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_LineSpacing( 
            /* [out][retval] */ float *value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_LineSpacingRule( 
            /* [out][retval] */ LONG *value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_ListAlignment( 
            /* [out][retval] */ LONG *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_ListAlignment( 
            /* [in] */ LONG value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_ListLevelIndex( 
            /* [out][retval] */ LONG *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_ListLevelIndex( 
            /* [in] */ LONG value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_ListStart( 
            /* [out][retval] */ LONG *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_ListStart( 
            /* [in] */ LONG value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_ListTab( 
            /* [out][retval] */ float *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_ListTab( 
            /* [in] */ float value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_ListType( 
            /* [out][retval] */ LONG *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_ListType( 
            /* [in] */ LONG value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_NoLineNumber( 
            /* [out][retval] */ LONG *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_NoLineNumber( 
            /* [in] */ LONG value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_PageBreakBefore( 
            /* [out][retval] */ LONG *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_PageBreakBefore( 
            /* [in] */ LONG value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_RightIndent( 
            /* [out][retval] */ float *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_RightIndent( 
            /* [in] */ float value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetIndents( 
            /* [in] */ float StartIndent,
            /* [in] */ float LeftIndent,
            /* [in] */ float RightIndent) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetLineSpacing( 
            /* [in] */ LONG LineSpacingRule,
            /* [in] */ float LineSpacing) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_SpaceAfter( 
            /* [out][retval] */ float *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_SpaceAfter( 
            /* [in] */ float value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_SpaceBefore( 
            /* [out][retval] */ float *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_SpaceBefore( 
            /* [in] */ float value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_WidowControl( 
            /* [out][retval] */ LONG *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_WidowControl( 
            /* [in] */ LONG value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_TabCount( 
            /* [out][retval] */ LONG *count) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AddTab( 
            /* [in] */ float tbPos,
            /* [in] */ LONG tbAlign,
            /* [in] */ LONG tbLeader) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ClearAllTabs( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DeleteTab( 
            /* [in] */ float tbPos) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetTab( 
            /* [in] */ LONG iTab,
            /* [out] */ float *ptbPos,
            /* [out] */ LONG *ptbAlign,
            /* [out] */ LONG *ptbLeader) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ITextParaVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITextPara * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITextPara * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITextPara * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ITextPara * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ITextPara * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ITextPara * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ITextPara * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Duplicate )( 
            ITextPara * This,
            /* [out][retval] */ ITextPara **para);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Duplicate )( 
            ITextPara * This,
            /* [in] */ ITextPara *para);
        
        HRESULT ( STDMETHODCALLTYPE *CanChange )( 
            ITextPara * This,
            /* [retval][out] */ LONG *pB);
        
        HRESULT ( STDMETHODCALLTYPE *IsEqual )( 
            ITextPara * This,
            /* [in] */ ITextPara *para,
            /* [out][retval] */ LONG *pB);
        
        HRESULT ( STDMETHODCALLTYPE *Reset )( 
            ITextPara * This,
            /* [in] */ LONG value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Style )( 
            ITextPara * This,
            /* [out][retval] */ LONG *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_Style )( 
            ITextPara * This,
            /* [in] */ LONG value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Alignment )( 
            ITextPara * This,
            /* [out][retval] */ LONG *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_Alignment )( 
            ITextPara * This,
            /* [in] */ LONG value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Hyphenation )( 
            ITextPara * This,
            /* [out][retval] */ LONG *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_Hyphenation )( 
            ITextPara * This,
            /* [in] */ LONG value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_FirstLineIndent )( 
            ITextPara * This,
            /* [out][retval] */ float *value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_KeepTogether )( 
            ITextPara * This,
            /* [out][retval] */ LONG *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_KeepTogether )( 
            ITextPara * This,
            /* [in] */ LONG value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_KeepWithNext )( 
            ITextPara * This,
            /* [out][retval] */ LONG *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_KeepWithNext )( 
            ITextPara * This,
            /* [in] */ LONG value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_LeftIndent )( 
            ITextPara * This,
            /* [out][retval] */ float *value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_LineSpacing )( 
            ITextPara * This,
            /* [out][retval] */ float *value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_LineSpacingRule )( 
            ITextPara * This,
            /* [out][retval] */ LONG *value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_ListAlignment )( 
            ITextPara * This,
            /* [out][retval] */ LONG *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_ListAlignment )( 
            ITextPara * This,
            /* [in] */ LONG value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_ListLevelIndex )( 
            ITextPara * This,
            /* [out][retval] */ LONG *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_ListLevelIndex )( 
            ITextPara * This,
            /* [in] */ LONG value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_ListStart )( 
            ITextPara * This,
            /* [out][retval] */ LONG *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_ListStart )( 
            ITextPara * This,
            /* [in] */ LONG value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_ListTab )( 
            ITextPara * This,
            /* [out][retval] */ float *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_ListTab )( 
            ITextPara * This,
            /* [in] */ float value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_ListType )( 
            ITextPara * This,
            /* [out][retval] */ LONG *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_ListType )( 
            ITextPara * This,
            /* [in] */ LONG value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_NoLineNumber )( 
            ITextPara * This,
            /* [out][retval] */ LONG *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_NoLineNumber )( 
            ITextPara * This,
            /* [in] */ LONG value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_PageBreakBefore )( 
            ITextPara * This,
            /* [out][retval] */ LONG *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_PageBreakBefore )( 
            ITextPara * This,
            /* [in] */ LONG value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_RightIndent )( 
            ITextPara * This,
            /* [out][retval] */ float *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_RightIndent )( 
            ITextPara * This,
            /* [in] */ float value);
        
        HRESULT ( STDMETHODCALLTYPE *SetIndents )( 
            ITextPara * This,
            /* [in] */ float StartIndent,
            /* [in] */ float LeftIndent,
            /* [in] */ float RightIndent);
        
        HRESULT ( STDMETHODCALLTYPE *SetLineSpacing )( 
            ITextPara * This,
            /* [in] */ LONG LineSpacingRule,
            /* [in] */ float LineSpacing);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_SpaceAfter )( 
            ITextPara * This,
            /* [out][retval] */ float *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_SpaceAfter )( 
            ITextPara * This,
            /* [in] */ float value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_SpaceBefore )( 
            ITextPara * This,
            /* [out][retval] */ float *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_SpaceBefore )( 
            ITextPara * This,
            /* [in] */ float value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_WidowControl )( 
            ITextPara * This,
            /* [out][retval] */ LONG *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_WidowControl )( 
            ITextPara * This,
            /* [in] */ LONG value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_TabCount )( 
            ITextPara * This,
            /* [out][retval] */ LONG *count);
        
        HRESULT ( STDMETHODCALLTYPE *AddTab )( 
            ITextPara * This,
            /* [in] */ float tbPos,
            /* [in] */ LONG tbAlign,
            /* [in] */ LONG tbLeader);
        
        HRESULT ( STDMETHODCALLTYPE *ClearAllTabs )( 
            ITextPara * This);
        
        HRESULT ( STDMETHODCALLTYPE *DeleteTab )( 
            ITextPara * This,
            /* [in] */ float tbPos);
        
        HRESULT ( STDMETHODCALLTYPE *GetTab )( 
            ITextPara * This,
            /* [in] */ LONG iTab,
            /* [out] */ float *ptbPos,
            /* [out] */ LONG *ptbAlign,
            /* [out] */ LONG *ptbLeader);
        
        END_INTERFACE
    } ITextParaVtbl;

    interface ITextPara
    {
        CONST_VTBL struct ITextParaVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITextPara_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ITextPara_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ITextPara_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ITextPara_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ITextPara_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ITextPara_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ITextPara_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ITextPara_get_Duplicate(This,para)	\
    ( (This)->lpVtbl -> get_Duplicate(This,para) ) 

#define ITextPara_put_Duplicate(This,para)	\
    ( (This)->lpVtbl -> put_Duplicate(This,para) ) 

#define ITextPara_CanChange(This,pB)	\
    ( (This)->lpVtbl -> CanChange(This,pB) ) 

#define ITextPara_IsEqual(This,para,pB)	\
    ( (This)->lpVtbl -> IsEqual(This,para,pB) ) 

#define ITextPara_Reset(This,value)	\
    ( (This)->lpVtbl -> Reset(This,value) ) 

#define ITextPara_get_Style(This,value)	\
    ( (This)->lpVtbl -> get_Style(This,value) ) 

#define ITextPara_put_Style(This,value)	\
    ( (This)->lpVtbl -> put_Style(This,value) ) 

#define ITextPara_get_Alignment(This,value)	\
    ( (This)->lpVtbl -> get_Alignment(This,value) ) 

#define ITextPara_put_Alignment(This,value)	\
    ( (This)->lpVtbl -> put_Alignment(This,value) ) 

#define ITextPara_get_Hyphenation(This,value)	\
    ( (This)->lpVtbl -> get_Hyphenation(This,value) ) 

#define ITextPara_put_Hyphenation(This,value)	\
    ( (This)->lpVtbl -> put_Hyphenation(This,value) ) 

#define ITextPara_get_FirstLineIndent(This,value)	\
    ( (This)->lpVtbl -> get_FirstLineIndent(This,value) ) 

#define ITextPara_get_KeepTogether(This,value)	\
    ( (This)->lpVtbl -> get_KeepTogether(This,value) ) 

#define ITextPara_put_KeepTogether(This,value)	\
    ( (This)->lpVtbl -> put_KeepTogether(This,value) ) 

#define ITextPara_get_KeepWithNext(This,value)	\
    ( (This)->lpVtbl -> get_KeepWithNext(This,value) ) 

#define ITextPara_put_KeepWithNext(This,value)	\
    ( (This)->lpVtbl -> put_KeepWithNext(This,value) ) 

#define ITextPara_get_LeftIndent(This,value)	\
    ( (This)->lpVtbl -> get_LeftIndent(This,value) ) 

#define ITextPara_get_LineSpacing(This,value)	\
    ( (This)->lpVtbl -> get_LineSpacing(This,value) ) 

#define ITextPara_get_LineSpacingRule(This,value)	\
    ( (This)->lpVtbl -> get_LineSpacingRule(This,value) ) 

#define ITextPara_get_ListAlignment(This,value)	\
    ( (This)->lpVtbl -> get_ListAlignment(This,value) ) 

#define ITextPara_put_ListAlignment(This,value)	\
    ( (This)->lpVtbl -> put_ListAlignment(This,value) ) 

#define ITextPara_get_ListLevelIndex(This,value)	\
    ( (This)->lpVtbl -> get_ListLevelIndex(This,value) ) 

#define ITextPara_put_ListLevelIndex(This,value)	\
    ( (This)->lpVtbl -> put_ListLevelIndex(This,value) ) 

#define ITextPara_get_ListStart(This,value)	\
    ( (This)->lpVtbl -> get_ListStart(This,value) ) 

#define ITextPara_put_ListStart(This,value)	\
    ( (This)->lpVtbl -> put_ListStart(This,value) ) 

#define ITextPara_get_ListTab(This,value)	\
    ( (This)->lpVtbl -> get_ListTab(This,value) ) 

#define ITextPara_put_ListTab(This,value)	\
    ( (This)->lpVtbl -> put_ListTab(This,value) ) 

#define ITextPara_get_ListType(This,value)	\
    ( (This)->lpVtbl -> get_ListType(This,value) ) 

#define ITextPara_put_ListType(This,value)	\
    ( (This)->lpVtbl -> put_ListType(This,value) ) 

#define ITextPara_get_NoLineNumber(This,value)	\
    ( (This)->lpVtbl -> get_NoLineNumber(This,value) ) 

#define ITextPara_put_NoLineNumber(This,value)	\
    ( (This)->lpVtbl -> put_NoLineNumber(This,value) ) 

#define ITextPara_get_PageBreakBefore(This,value)	\
    ( (This)->lpVtbl -> get_PageBreakBefore(This,value) ) 

#define ITextPara_put_PageBreakBefore(This,value)	\
    ( (This)->lpVtbl -> put_PageBreakBefore(This,value) ) 

#define ITextPara_get_RightIndent(This,value)	\
    ( (This)->lpVtbl -> get_RightIndent(This,value) ) 

#define ITextPara_put_RightIndent(This,value)	\
    ( (This)->lpVtbl -> put_RightIndent(This,value) ) 

#define ITextPara_SetIndents(This,StartIndent,LeftIndent,RightIndent)	\
    ( (This)->lpVtbl -> SetIndents(This,StartIndent,LeftIndent,RightIndent) ) 

#define ITextPara_SetLineSpacing(This,LineSpacingRule,LineSpacing)	\
    ( (This)->lpVtbl -> SetLineSpacing(This,LineSpacingRule,LineSpacing) ) 

#define ITextPara_get_SpaceAfter(This,value)	\
    ( (This)->lpVtbl -> get_SpaceAfter(This,value) ) 

#define ITextPara_put_SpaceAfter(This,value)	\
    ( (This)->lpVtbl -> put_SpaceAfter(This,value) ) 

#define ITextPara_get_SpaceBefore(This,value)	\
    ( (This)->lpVtbl -> get_SpaceBefore(This,value) ) 

#define ITextPara_put_SpaceBefore(This,value)	\
    ( (This)->lpVtbl -> put_SpaceBefore(This,value) ) 

#define ITextPara_get_WidowControl(This,value)	\
    ( (This)->lpVtbl -> get_WidowControl(This,value) ) 

#define ITextPara_put_WidowControl(This,value)	\
    ( (This)->lpVtbl -> put_WidowControl(This,value) ) 

#define ITextPara_get_TabCount(This,count)	\
    ( (This)->lpVtbl -> get_TabCount(This,count) ) 

#define ITextPara_AddTab(This,tbPos,tbAlign,tbLeader)	\
    ( (This)->lpVtbl -> AddTab(This,tbPos,tbAlign,tbLeader) ) 

#define ITextPara_ClearAllTabs(This)	\
    ( (This)->lpVtbl -> ClearAllTabs(This) ) 

#define ITextPara_DeleteTab(This,tbPos)	\
    ( (This)->lpVtbl -> DeleteTab(This,tbPos) ) 

#define ITextPara_GetTab(This,iTab,ptbPos,ptbAlign,ptbLeader)	\
    ( (This)->lpVtbl -> GetTab(This,iTab,ptbPos,ptbAlign,ptbLeader) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ITextPara_INTERFACE_DEFINED__ */

#endif /* __tom_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


