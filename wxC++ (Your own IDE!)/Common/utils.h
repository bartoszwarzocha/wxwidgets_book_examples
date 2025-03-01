///////////////////////////////////////////////////////////////////////////////
// Name:        utils.h
// Purpose:     My wxWidgets app
// Author:      Name Surname <name.surname@email.org>
// Created:     2025-01-01
// Copyright:   (c) 2025 by Name Surname
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _UTILS_H_
#define _UTILS_H_

#include <wx/stdpaths.h>

// Settings
#define _INSTALLER_ 0 // Compilation for installer or installation package

//---------------------------------------------------------------------------
// Function returns the path to the program file depending on the platform
// and compilation scenario
//---------------------------------------------------------------------------
wxString wxGetAppFile(wxString file)
{
    wxString path = wxPathOnly(wxStandardPaths::Get().GetExecutablePath());
	
#if _INSTALLER_ && defined (__WXGTK__)
    path = wxStandardPaths::Get().GetDataDir();
#endif

    path = path + wxT("/") + file;
    path.Replace("\\","/",true);
    path.Replace("//","/",true);

    return path;
}

wxString wxGetWXKeyWords()
{
	// wxWidgets 3.1.0 - plase update if you need
	wxString wxA_words = wxT("wxAboutDialogInfo wxAcceleratorEntry wxAcceleratorTable " \
        "wxAccessible wxActivateEvent wxActiveXContainer wxActiveXEvent wxActivityIndicator " \
        "wxAddRemoveAdaptor wxAddRemoveCtrl wxAffineMatrix2D wxAffineMatrix2DBase wxAnimation " \
        "wxAnimationCtrl wxAny wxAnyButton wxAnyValueBuffer wxAnyValueType wxApp wxAppConsole " \
        "wxAppProgressIndicator wxAppTraits wxArchiveClassFactory wxArchiveEntry " \
        "wxArchiveFSHandler wxArchiveInputStream wxArchiveIterator wxArchiveNotifier " \
        "wxArchiveOutputStream wxArray wxArrayString wxArtProvider  wxAuiDefaultTabArt " \
        "wxAuiDefaultToolBarArt wxAuiDockArt wxAuiManager wxAuiManagerEvent wxAuiNotebook " \
        "wxAuiNotebookEvent wxAuiPaneInfo wxAuiSimpleTabArt wxAuiTabArt wxAuiTabContainer " \
        "wxAuiTabContainerButton wxAuiToolBar wxAuiToolBarArt wxAuiToolBarEvent " \
        "wxAuiToolBarItem wxAutoBufferedPaintDC wxAutomationObject ");

	wxString wxB_words = wxT("wxBannerWindow wxBitmap wxBitmapButton wxBitmapComboBox " \
        "wxBitmapDataObject wxBitmapHandler wxBitmapToggleButton wxBookCtrlBase " \
        "wxBookCtrlEvent wxBoxSizer wxBrush wxBrushList wxBufferedDC wxBitmapToggleButton " \
        "wxBookCtrlBase wxBookCtrlEvent wxBoxSizer wxBrush wxBrushList wxBufferedDC " \
        "wxBufferedInputStream wxBufferedOutputStream wxBufferedPaintDC wxBusyCursor " \
        "wxBusyInfo wxBusyInfoFlags wxButton ");
	
	wxString wxC_words = wxT("wxCalculateLayoutEvent wxCalendarCtrl wxCalendarDateAttr " \
		"wxCalendarEvent wxCaret wxCharBuffer wxCharTypeBuffer wxCheckBox wxCheckListBox " \
		"wxChildFocusEvent wxChoice wxChoicebook wxClassInfo wxClient wxClientData " \
		"wxClientDataContainer wxClientDC wxClipboard wxClipboardTextEvent wxCloseEvent " \
		"wxCmdLineArg wxCmdLineArgs wxCmdLineEntryDesc wxCmdLineParser wxCollapsibleHeaderCtrl " \
		"wxCollapsiblePane wxCollapsiblePaneEvent wxColour wxColourData wxColourDatabase " \
		"wxColourDialog wxColourPickerCtrl wxColourPickerEvent wxComboBox wxComboCtrl " \
		"wxComboCtrlFeatures wxComboPopup wxCommand wxCommandEvent wxCommandLinkButton " \
		"xCommandProcessor wxCondition wxConfigBase wxConfigPathChanger wxConnection " \
		"wxConnectionBase wxContextHelp wxContextHelpButton wxContextMenuEvent wxControl " \
		"wxControlWithItems wxConvAuto wxCountingOutputStream wxCriticalSection " \
		"wxCriticalSectionLocker wxCSConv wxCursor wxCustomBackgroundWindow wxCustomDataObject");
	
	wxString wxD_words = wxT("wxDataFormat wxDatagramSocket wxDataInputStream wxDataObject " \
		"wxDataObjectComposite wxDataObjectSimple wxDataOutputStream wxDataViewBitmapRenderer " \
		"wxDataViewChoiceByIndexRenderer wxDataViewChoiceRenderer wxDataViewColumn wxDataViewCtrl " \
		"wxDataViewCustomRenderer wxDataViewDateRenderer wxDataViewEvent wxDataViewIconText " \
		"wxDataViewIconTextRenderer wxDataViewIndexListModel wxDataViewItem wxDataViewItemAttr " \
		"wxDataViewListCtrl wxDataViewListModel wxDataViewListStore wxDataViewModel " \
		"wxDataViewModelNotifier wxDataViewProgressRenderer wxDataViewRenderer wxDataViewSpinRenderer " \
		"wxDataViewTextRenderer wxDataViewToggleRenderer wxDataViewTreeCtrl wxDataViewTreeStore " \
		"wxDataViewVirtualListModel wxDateEvent wxDatePickerCtrl wxDateSpan wxDateTime TimeZone " \
		"Tm wxDateTimeHolidayAuthority wxDateTimeWorkDays wxDC wxDCBrushChanger wxDCClipper " \
		"wxDCFontChanger wxDCOverlay wxDCPenChanger wxDCTextColourChanger wxDDEClient wxDDEConnection " \
		"wxDDEServer wxDebugContext wxDebugReport wxDebugReportCompress wxDebugReportPreview " \
		"wxDebugReportPreviewStd wxDebugReportUpload wxDelegateRendererNative wxDialog " \
		"wxDialogLayoutAdapter wxDialUpEvent wxDialUpManager wxDir wxDirDialog wxDirFilterListCtrl " \
		"wxDirPickerCtrl wxDirTraverser wxDisplay wxDisplayChangedEvent wxDocChildFrame " \
		"wxDocManager wxDocMDIChildFrame wxDocMDIParentFrame wxDocParentFrame wxDocTemplate " \
		"wxDocument wxDragImage wxDropFilesEvent wxDropSource wxDropTarget wxDynamicLibrary " \
		"wxDynamicLibraryDetails");
	
	wxString wxE_words = wxT("wxEditableListBox wxEncodingConverter wxEraseEvent wxEvent " \
		"wxEventBlocker wxEventFilter wxEventLoopActivator wxEventLoopBase wxEvtHandler wxExecuteEnv " \
		"wxExtHelpController");
	
	wxString wxF_words = wxT("wxFFile wxFFileInputStream wxFFileOutputStream wxFFileStream " \
		"wxFile wxFileConfig wxFileCtrl wxFileCtrlEvent wxFileDataObject wxFileDialog " \
		"wxFileDirPickerEvent wxFileDropTarget wxFileHistory wxFileInputStream wxFileName " \
		"wxFileOutputStream wxFilePickerCtrl wxFileStream wxFileSystem wxFileSystemHandler " \
		"wxFileSystemWatcher wxFileSystemWatcherEvent wxFileTranslationsLoader wxFileType " \
		"MessageParameters wxFileTypeInfo wxFilterClassFactory wxFilterFSHandler " \
		"wxFilterInputStream wxFilterOutputStream wxFindDialogEvent wxFindReplaceData " \
		"wxFindReplaceDialog wxFlexGridSizer wxFloatingPointValidator wxFocusEvent wxFont " \
		"wxFontData wxFontDialog wxFontEnumerator wxFontInfo wxFontList wxFontMapper " \
		"wxFontMetrics wxFontPickerCtrl wxFontPickerEvent wxFrame wxFSFile wxFSInputStream " \
		"wxFSVolume wxFTP");
	
	wxString wxG_words = wxT("wxGauge wxGBPosition wxGBSizerItem wxGBSpan wxGCDC " \
		"wxGDIObject wxGenericAboutDialog wxGenericDirCtrl wxGenericProgressDialog " \
		"wxGenericValidator wxGLAttribsBase wxGLAttributes wxGLCanvas wxGLContext " \
		"wxGLContextAttrs wxGraphicsBitmap wxGraphicsBrush wxGraphicsContext wxGraphicsFont " \
		"wxGraphicsGradientStop wxGraphicsGradientStops wxGraphicsMatrix wxGraphicsObject " \
		"wxGraphicsPath wxGraphicsPen wxGraphicsRenderer wxGrid wxGridBagSizer wxGridCellAttr " \
		"wxGridCellAttrProvider wxGridCellAutoWrapStringEditor wxGridCellAutoWrapStringRenderer " \
		"wxGridCellBoolEditor wxGridCellBoolRenderer wxGridCellChoiceEditor wxGridCellCoords " \
		"wxGridCellDateTimeRenderer wxGridCellEditor wxGridCellEnumEditor wxGridCellEnumRenderer " \
		"wxGridCellFloatEditor wxGridCellFloatRenderer wxGridCellNumberEditor wxGridCellNumberRenderer " \
		"wxGridCellRenderer wxGridCellStringRenderer wxGridCellTextEditor wxGridColumnHeaderRenderer " \
		"wxGridColumnHeaderRendererDefault wxGridCornerHeaderRenderer wxGridCornerHeaderRendererDefault " \
		"wxGridEditorCreatedEvent wxGridEvent wxGridHeaderLabelsRenderer wxGridRangeSelectEvent " \
		"wxGridRowHeaderRenderer wxGridRowHeaderRendererDefault wxGridSizeEvent wxGridSizer " \
		"wxGridSizesInfo wxGridStringTable wxGridTableBase wxGridTableMessage wxGridUpdateLocker " \
		"wxGUIEventLoop");
	
	wxString wxH_words = wxT("wxHashMap wxHashSet wxHashTable wxHeaderButtonParams " \
		"wxHeaderColumn wxHeaderColumnSimple wxHeaderCtrl wxHeaderCtrlEvent wxHeaderCtrlSimple " \
		"wxHelpController wxHelpControllerBase wxHelpControllerHelpProvider wxHelpEvent " \
		"wxHelpProvider wxHScrolledWindow wxHtmlBookRecord wxHtmlCell wxHtmlCellEvent " \
		"wxHtmlColourCell wxHtmlContainerCell wxHTMLDataObject wxHtmlDCRenderer wxHtmlEasyPrinting " \
		"wxHtmlFilter wxHtmlFontCell wxHtmlHelpController wxHtmlHelpData wxHtmlHelpDataItem " \
		"wxHtmlHelpDialog wxHtmlHelpFrame wxHtmlHelpWindow wxHtmlLinkEvent wxHtmlLinkInfo " \
		"wxHtmlListBox wxHtmlModalHelp wxHtmlParser wxHtmlPrintout wxHtmlRenderingInfo " \
		"wxHtmlRenderingState wxHtmlRenderingStyle wxHtmlSelection wxHtmlTag wxHtmlTagHandler " \
		"wxHtmlTagsModule wxHtmlWidgetCell wxHtmlWindow wxHtmlWindowInterface wxHtmlWinParser " \
		"wxHtmlWinTagHandler wxHtmlWordCell wxHtmlWordWithTabsCell wxHTTP wxHVScrolledWindow " \
		"wxHyperlinkCtrl wxHyperlinkEvent");
		
	wxString wxI_words = wxT("wxIcon wxIconBundle wxIconizeEvent wxIconLocation wxIdleEvent " \
		"wxIdManager wxImage HSVValue RGBValue wxImageHandler wxImageHistogram wxImageList " \
		"wxIndividualLayoutConstraint wxInfoBar wxInitDialogEvent wxInitializer wxInputStream " \
		"wxIntegerValidator wxInternetFSHandler wxIPaddress wxIPV4address wxItemContainer " \
		"wxItemContainerImmutable");
	
	wxString wxJ_words = wxT("wxJoystick wxJoystickEvent");
	
	wxString wxK_words = wxT("wxKeyboardState wxKeyEvent");
	
	wxString wxL_words = wxT("wxLanguageInfo wxLayoutAlgorithm wxLayoutConstraints " \
		"wxLinuxDistributionInfo wxList wxListbook wxListBox wxListCtrl wxListEvent wxListItem " \
		"wxListItemAttr wxListView wxLocale wxLog wxLogBuffer wxLogChain wxLogFormatter wxLogGui " \
		"wxLogInterposer wxLogInterposerTemp wxLogNull wxLogRecordInfo wxLogStderr wxLogStream " \
		"wxLogTextCtrl wxLogWindow wxLongLong");
	
	wxString wxM_words = wxT("wxMask wxMatrix2D wxMaximizeEvent wxMBConv wxMBConvUTF16 " \
		"wxMBConvUTF32 wxMBConvUTF7 wxMBConvUTF8 wxMDIChildFrame wxMDIClientWindow wxMDIParentFrame " \
		"wxMediaCtrl wxMediaEvent wxMemoryBuffer wxMemoryDC wxMemoryFSHandler wxMemoryInputStream " \
		"wxMemoryOutputStream wxMenu wxMenuBar wxMenuEvent wxMenuItem wxMessageDialog ButtonLabel " \
		"wxMessageOutput wxMessageOutputBest wxMessageOutputDebug wxMessageOutputMessageBox " \
		"wxMessageOutputStderr wxMessageQueue wxMetafile wxMetafileDC wxMimeTypesManager wxMiniFrame " \
		"wxMirrorDC wxModalDialogHook wxModule wxMouseCaptureChangedEvent wxMouseCaptureLostEvent " \
		"wxMouseEvent wxMouseEventsManager wxMouseState wxMoveEvent wxMsgCatalog wxMultiChoiceDialog " \
		"wxMutex wxMutexLocker");
	
	wxString wxN_words = wxT("wxNativeWindow wxNavigationEnabled wxNavigationKeyEvent wxNode " \
		"wxNonOwnedWindow wxNotebook wxNotificationMessage wxNotifyEvent wxNumberFormatter " \
		"wxNumValidator");
	
	wxString wxO_words = wxT("wxObject wxObjectDataPtr wxObjectRefData wxOutputStream " \
		"wxOverlay wxOwnerDrawnComboBox");
		
	wxString wxP_words = wxT("wxPageSetupDialog wxPageSetupDialogData wxPaintDC wxPaintEvent " \
		"wxPalette wxPaletteChangedEvent wxPanel wxPasswordEntryDialog wxPathList wxPen wxPenList " \
		"wxPersistenceManager wxPersistentBookCtrl wxPersistentObject wxPersistentTLW " \
		"wxPersistentTreeBookCtrl wxPersistentWindow wxPGCell wxPGChoices wxPGEditor wxPGMultiButton " \
		"wxPGProperty wxPGValidationInfo wxPGVIterator wxPickerBase wxPixelData Iterator wxPlatformInfo " \
		"wxPoint wxPoint2DDouble wxPoint2DInt wxPopupTransientWindow wxPopupWindow wxPosition wxPostScriptDC " \
		"wxPowerEvent wxPowerResource wxPowerResourceBlocker wxPreferencesEditor wxPreferencesPage " \
		"wxPreviewCanvas wxPreviewControlBar wxPreviewFrame wxPrintAbortDialog wxPrintData wxPrintDialog " \
		"wxPrintDialogData wxPrinter wxPrinterDC wxPrintout wxPrintPreview wxProcess wxProcessEvent " \
		"wxProgressDialog wxPropagateOnce wxPropagationDisabler wxPropertyGrid wxPropertyGridEvent " \
		"wxPropertyGridHitTestResult wxPropertyGridInterface wxPropertyGridIterator wxPropertyGridManager " \
		"wxPropertyGridPage wxPropertySheetDialog wxProtocol wxProtocolLog");
	
	wxString wxQ_words = wxT("wxQuantize Performs wxQueryLayoutInfoEvent wxQueryNewPaletteEvent");
	
	wxString wxR_words = wxT("wxRadioBox wxRadioButton wxRealPoint wxRearrangeCtrl wxRearrangeDialog " \
		"wxRearrangeList wxRect wxRect2DDouble wxRect2DInt wxRecursionGuard wxRecursionGuardFlag wxRefCounter " \
		"wxRegConfig wxRegEx wxRegion wxRegionIterator wxRegKey wxRendererNative wxRendererVersion " \
		"wxResourceTranslationsLoader wxRibbonArtProvider wxRibbonBar wxRibbonBarEvent wxRibbonButtonBar " \
		"wxRibbonButtonBarEvent wxRibbonControl wxRibbonGallery wxRibbonGalleryEvent wxRibbonPage " \
		"wxRibbonPanel wxRibbonPanelEvent wxRibbonToolBar wxRichMessageDialog wxRichTextAction wxRichTextAttr " \
		"wxRichTextBox wxRichTextBuffer wxRichTextBufferDataObject wxRichTextCell wxRichTextCharacterStyleDefinition " \
		"wxRichTextCommand wxRichTextCompositeObject wxRichTextContextMenuPropertiesInfo wxRichTextCtrl " \
		"wxRichTextDrawingContext wxRichTextDrawingHandler wxRichTextEvent wxRichTextField wxRichTextFieldType " \
		"wxRichTextFieldTypeStandard wxRichTextFileHandler wxRichTextFontTable wxRichTextFormattingDialog " \
		"wxRichTextFormattingDialogFactory wxRichTextHeaderFooterData wxRichTextHTMLHandler wxRichTextImage " \
		"wxRichTextImageBlock wxRichTextLine wxRichTextListStyleDefinition wxRichTextObject wxRichTextObjectAddress " \
		"wxRichTextParagraph wxRichTextParagraphLayoutBox wxRichTextParagraphStyleDefinition wxRichTextPlainText " \
		"wxRichTextPlainTextHandler wxRichTextPrinting wxRichTextPrintout wxRichTextProperties wxRichTextRange " \
		"wxRichTextRenderer wxRichTextSelection wxRichTextStdRenderer wxRichTextStyleComboCtrl " \
		"wxRichTextStyleDefinition wxRichTextStyleListBox wxRichTextStyleListCtrl wxRichTextStyleOrganiserDialog " \
		"wxRichTextStyleSheet wxRichTextTable wxRichTextTableBlock wxRichTextXMLHandler wxRichToolTip");
	
	wxString wxS_words = wxT("wxSashEvent wxSashLayoutWindow wxSashWindow wxScopedArray wxScopedCharTypeBuffer " \
		"wxScopedPtr wxScopedPtr wxScopedTiedPtr wxScopeGuard wxScreenDC wxScrollBar wxScrolled wxScrollEvent " \
		"wxScrollWinEvent wxSearchCtrl wxSemaphore wxServer wxSetCursorEvent wxSettableHeaderColumn wxSharedPtr " \
		"wxShowEvent wxSimplebook wxSimpleHelpProvider wxSimpleHtmlListBox wxSingleChoiceDialog wxSingleInstanceChecker " \
		"wxSize wxSizeEvent wxSizer wxSizerFlags wxSizerItem wxSizerXmlHandler wxSlider wxSockAddress wxSocketBase " \
		"wxSocketClient wxSocketEvent wxSocketInputStream wxSocketOutputStream wxSocketServer wxSortedArrayString " \
		"wxSound wxSpinButton wxSpinCtrl wxSpinCtrlDouble wxSpinDoubleEvent wxSpinEvent wxSplashScreen wxSplitterEvent " \
		"wxSplitterRenderParams wxSplitterWindow wxStack wxStackFrame wxStackWalker wxStandardPaths wxStaticBitmap " \
		"wxStaticBox wxStaticBoxSizer wxStaticLine wxStaticText wxStatusBar wxStatusBarPane wxStdDialogButtonSizer " \
		"wxStdInputStream wxStdInputStreamBuffer wxStdOutputStream wxStdOutputStreamBuffer wxStockPreferencesPage " \
		"wxStopWatch wxStreamBase wxStreamBuffer wxStreamToTextRedirector wxString wxStringBuffer wxStringBufferLength " \
		"wxStringClientData wxStringInputStream wxStringOutputStream wxStringTokenizer wxStyledTextCtrl " \
		"wxStyledTextEvent wxSVGBitmapEmbedHandler wxSVGBitmapFileHandler wxSVGBitmapHandler wxSVGFileDC " \
		"wxSymbolPickerDialog wxSysColourChangedEvent wxSystemOptions wxSystemSettings wxSystemThemedControl");
	
	wxString wxT_words = wxT("wxTarClassFactory wxTarEntry wxTarInputStream wxTarOutputStream wxTaskBarButton " \
		"wxTaskBarIcon wxTaskBarIconEvent wxTaskBarJumpList wxTaskBarJumpListCategory wxTaskBarJumpListItem " \
		"wxTCPClient wxTCPConnection wxTCPServer wxTempFile wxTempFileOutputStream wxTextAttr wxTextAttrBorder " \
		"wxTextAttrBorders wxTextAttrDimension wxTextAttrDimensionConverter wxTextAttrDimensions " \
		"wxTextAttrShadow wxTextAttrSize wxTextBoxAttr wxTextCompleter wxTextCompleterSimple wxTextCtrl " \
		"wxTextDataObject wxTextDropTarget wxTextEntry wxTextEntryDialog wxTextFile wxTextInputStream " \
		"wxTextOutputStream wxTextUrlEvent wxTextValidator wxTextWrapper wxThread wxThreadEvent wxThreadHelper " \
		"wxThumbBarButton wxTimePickerCtrl wxTimer wxTimerEvent wxTimerRunner wxTimeSpan wxTipProvider " \
		"wxTipWindow wxToggleButton wxToolBar wxToolBarToolBase wxToolbook wxToolTip wxTopLevelWindow " \
		"wxTrackable wxTransform2D wxTranslations wxTranslationsLoader wxTreebook wxTreeCtrl wxTreeEvent " \
		"wxTreeItemData wxTreeItemId wxTreeListCtrl wxTreeListEvent wxTreeListItem wxTreeListItemComparator");
	
	wxString wxU_words = wxT("wxUIActionSimulator wxULongLong wxUniChar wxUniCharRef " \
		"wxUpdateUIEvent wxURI wxURL wxURLDataObject wxUString");
	
	wxString wxV_words = wxT("wxValidator wxVarHScrollHelper wxVarHVScrollHelper wxVariant " \
		"wxVariantData wxVariantDataCurrency wxVariantDataErrorCode wxVariantDataSafeArray " \
		"wxVarScrollHelperBase wxVarVScrollHelper wxVector wxVersionInfo wxVideoMode wxView " \
		"wxVisualAttributes wxVListBox wxVScrolledWindow");
		
	wxString wxW_words = wxT("wxWCharBuffer wxWeakRef wxWeakRefDynamic wxWebKitBeforeLoadEvent " \
		"wxWebKitCtrl wxWebKitNewWindowEvent wxWebKitStateChangedEvent wxWebView wxWebViewArchiveHandler " \
		"wxWebViewEvent wxWebViewFactory wxWebViewFSHandler wxWebViewHandler wxWebViewHistoryItem wxWindow " \
		"ChildrenRepositioningGuard wxWindowCreateEvent wxWindowDC wxWindowDestroyEvent wxWindowDisabler " \
		"wxWindowModalDialogEvent wxWindowPtr wxWindowUpdateLocker wxWithImages wxWizard wxWizardEvent " \
		"wxWizardPage wxWizardPageSimple wxWrapperInputStream wxWrapSizer");

	wxString wxX_words = wxT("wxXLocale wxXmlAttribute wxXmlDoctype wxXmlDocument wxXmlNode " \
        "wxXmlResource wxXmlResourceHandler ");

	wxString wxZ_words = wxT("wxZipClassFactory wxZipEntry wxZipInputStream wxZipNotifier " \
        "wxZipOutputStream wxZlibInputStream wxZlibOutputStream");

	return wxA_words + wxB_words + wxC_words + wxD_words + wxE_words + wxF_words + 
        wxG_words + wxH_words + wxI_words + wxJ_words + wxK_words + wxL_words + wxM_words +
        wxN_words + wxO_words + wxP_words + wxQ_words + wxR_words + wxS_words + wxT_words +
        wxU_words + wxV_words + wxW_words + wxX_words + wxZ_words;
}

// Keywords of C++11 - please update if you need
wxString wxGetCPP11KeyWords()
{
	wxString Cpp11_words_set1 = wxT("alignas alignof and and_eq asm atomic_cancel " \
        "atomic_commit atomic_noexcept auto bitand bitor bool break case catch char char16_t " \
        "char32_t class compl concept const constexpr const_cast continue decltype default " \
        "delete do double dynamic_cast else enum explicit export extern false float for friend " \
        "goto if import inline int long module mutable namespace new noexcept not not_eq " \
        "nullptr operator or or_eq private protected public register reinterpret_cast requires " \
        "return short signed sizeof static static_assert static_cast struct switch " \
        "synchronized template this thread_local throw true try typedef typeid typename union " \
        "unsigned using virtual void volatile wchar_t while xor xor_eq");

	wxString Cpp11_words_set2 = wxT("override final transaction_safe transaction_safe_dynamic");

	wxString Cpp11_words_set3 = wxT("");

	return Cpp11_words_set1 + Cpp11_words_set2 + Cpp11_words_set3;
}

#endif
