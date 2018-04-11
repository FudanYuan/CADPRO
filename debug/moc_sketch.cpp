/****************************************************************************
** Meta object code from reading C++ file 'sketch.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../cad/sketch.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sketch.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Sketch_t {
    QByteArrayData data[219];
    char stringdata0[4906];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Sketch_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Sketch_t qt_meta_stringdata_Sketch = {
    {
QT_MOC_LITERAL(0, 0, 6), // "Sketch"
QT_MOC_LITERAL(1, 7, 13), // "configChanged"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 4), // "name"
QT_MOC_LITERAL(4, 27, 5), // "value"
QT_MOC_LITERAL(5, 33, 15), // "onActionFileNew"
QT_MOC_LITERAL(6, 49, 16), // "onActionFileOpen"
QT_MOC_LITERAL(7, 66, 16), // "onActionFileSave"
QT_MOC_LITERAL(8, 83, 18), // "onActionFileSaveAs"
QT_MOC_LITERAL(9, 102, 19), // "onActionFileSaveAll"
QT_MOC_LITERAL(10, 122, 17), // "onActionFilePrint"
QT_MOC_LITERAL(11, 140, 24), // "onActionFilePrintPreview"
QT_MOC_LITERAL(12, 165, 22), // "onActionFilePrintSetup"
QT_MOC_LITERAL(13, 188, 21), // "onActionFileImportDXF"
QT_MOC_LITERAL(14, 210, 21), // "onActionFileImportDVS"
QT_MOC_LITERAL(15, 232, 21), // "onActionFileImportTEF"
QT_MOC_LITERAL(16, 254, 21), // "onActionFileExportDXF"
QT_MOC_LITERAL(17, 276, 21), // "onActionFileExportDVS"
QT_MOC_LITERAL(18, 298, 21), // "onActionFileExportTEF"
QT_MOC_LITERAL(19, 320, 25), // "onActionFileConfiguration"
QT_MOC_LITERAL(20, 346, 16), // "onActionFileExit"
QT_MOC_LITERAL(21, 363, 16), // "onActionDrawLine"
QT_MOC_LITERAL(22, 380, 19), // "onActionDrawEllipse"
QT_MOC_LITERAL(23, 400, 16), // "onActionDrawRect"
QT_MOC_LITERAL(24, 417, 22), // "onActionDrawMiddleAxis"
QT_MOC_LITERAL(25, 440, 20), // "onActionDrawPolyline"
QT_MOC_LITERAL(26, 461, 18), // "onActionDrawCircle"
QT_MOC_LITERAL(27, 480, 22), // "onActionDrawArcBy3Pnts"
QT_MOC_LITERAL(28, 503, 23), // "onActionDrawArcBy3Pnts2"
QT_MOC_LITERAL(29, 527, 21), // "onActionDrawTrapezium"
QT_MOC_LITERAL(30, 549, 19), // "onActionDrawPolygon"
QT_MOC_LITERAL(31, 569, 20), // "onActionDrawStabHole"
QT_MOC_LITERAL(32, 590, 21), // "onActionDrawReference"
QT_MOC_LITERAL(33, 612, 18), // "onActionDrawEyelet"
QT_MOC_LITERAL(34, 631, 28), // "onActionDrawPatternDirection"
QT_MOC_LITERAL(35, 660, 21), // "onActionDrawShankLine"
QT_MOC_LITERAL(36, 682, 25), // "onActionDrawPerpendicular"
QT_MOC_LITERAL(37, 708, 17), // "onActionDrawImage"
QT_MOC_LITERAL(38, 726, 23), // "onActionDrawImageInsert"
QT_MOC_LITERAL(39, 750, 23), // "onActionDrawImageDelete"
QT_MOC_LITERAL(40, 774, 21), // "onActionDrawImageCrop"
QT_MOC_LITERAL(41, 796, 26), // "onActionDrawImageCalibrate"
QT_MOC_LITERAL(42, 823, 27), // "onActionDrawImageAdjustment"
QT_MOC_LITERAL(43, 851, 29), // "onActionDrawDigitizerActivate"
QT_MOC_LITERAL(44, 881, 32), // "onActionDrawDigitizerSetPosition"
QT_MOC_LITERAL(45, 914, 29), // "onActionDrawDigitizerConfigue"
QT_MOC_LITERAL(46, 944, 39), // "onActionDrawScannerVectorizeF..."
QT_MOC_LITERAL(47, 984, 31), // "onActionDrawScannerSelectSource"
QT_MOC_LITERAL(48, 1016, 33), // "onActionDrawScannerVectorizeI..."
QT_MOC_LITERAL(49, 1050, 20), // "onActionInsertOffset"
QT_MOC_LITERAL(50, 1071, 28), // "onActionInsertAdvancedOffset"
QT_MOC_LITERAL(51, 1100, 18), // "onActionInsertText"
QT_MOC_LITERAL(52, 1119, 20), // "onActionInsertFillet"
QT_MOC_LITERAL(53, 1140, 19), // "onActionInsertNotch"
QT_MOC_LITERAL(54, 1160, 26), // "onActionInsertNotchesGroup"
QT_MOC_LITERAL(55, 1187, 34), // "onActionInsertNotchesAxisTied..."
QT_MOC_LITERAL(56, 1222, 23), // "onActionInsertSizeNotch"
QT_MOC_LITERAL(57, 1246, 20), // "onActionInsertRemark"
QT_MOC_LITERAL(58, 1267, 20), // "onActionInsertSmooth"
QT_MOC_LITERAL(59, 1288, 20), // "onActionInsertMargin"
QT_MOC_LITERAL(60, 1309, 19), // "onActionInsertCanel"
QT_MOC_LITERAL(61, 1329, 19), // "onActionInsertBlock"
QT_MOC_LITERAL(62, 1349, 28), // "onActionInsertHolesOverALine"
QT_MOC_LITERAL(63, 1378, 28), // "onActionInsertArrayRectangle"
QT_MOC_LITERAL(64, 1407, 27), // "onActionInsertArrayCircular"
QT_MOC_LITERAL(65, 1435, 18), // "onActionModifyUndo"
QT_MOC_LITERAL(66, 1454, 18), // "onActionModifyRedo"
QT_MOC_LITERAL(67, 1473, 19), // "onActionModifyClear"
QT_MOC_LITERAL(68, 1493, 20), // "onActionModifyDelete"
QT_MOC_LITERAL(69, 1514, 17), // "onActionModifyCut"
QT_MOC_LITERAL(70, 1532, 18), // "onActionModifyCopy"
QT_MOC_LITERAL(71, 1551, 19), // "onActionModifyPaste"
QT_MOC_LITERAL(72, 1571, 26), // "onActionModifyPasteAsSlave"
QT_MOC_LITERAL(73, 1598, 27), // "onActionModifyTransformMove"
QT_MOC_LITERAL(74, 1626, 29), // "onActionModifyTransformRotate"
QT_MOC_LITERAL(75, 1656, 44), // "onActionModifyTransformRotate..."
QT_MOC_LITERAL(76, 1701, 34), // "onActionModifyTransformRotate..."
QT_MOC_LITERAL(77, 1736, 28), // "onActionModifyTransformScale"
QT_MOC_LITERAL(78, 1765, 29), // "onActionModifyTransformMirror"
QT_MOC_LITERAL(79, 1795, 29), // "onActionModifyTransformStrech"
QT_MOC_LITERAL(80, 1825, 30), // "onActionModifyTransformExplode"
QT_MOC_LITERAL(81, 1856, 27), // "onActionModifyTransformJoin"
QT_MOC_LITERAL(82, 1884, 28), // "onActionModifyTransformBreak"
QT_MOC_LITERAL(83, 1913, 29), // "onActionModifyTransformfillet"
QT_MOC_LITERAL(84, 1943, 27), // "onActionModifyTransformVCut"
QT_MOC_LITERAL(85, 1971, 29), // "onActionModifyTransformSpline"
QT_MOC_LITERAL(86, 2001, 18), // "onActionModifyTrim"
QT_MOC_LITERAL(87, 2020, 20), // "onActionModifyExtend"
QT_MOC_LITERAL(88, 2041, 22), // "onActionModifyEndpoint"
QT_MOC_LITERAL(89, 2064, 28), // "onActionModifyAdjustPolyline"
QT_MOC_LITERAL(90, 2093, 24), // "onActionModifyStartpoint"
QT_MOC_LITERAL(91, 2118, 22), // "onActionModifySoftenup"
QT_MOC_LITERAL(92, 2141, 18), // "onActionModifyWarp"
QT_MOC_LITERAL(93, 2160, 28), // "onActionModifyKnotsReduction"
QT_MOC_LITERAL(94, 2189, 25), // "onActionModifyReplacePart"
QT_MOC_LITERAL(95, 2215, 22), // "onActionModifyChangeTo"
QT_MOC_LITERAL(96, 2238, 28), // "onActionModifyMakeMasterLine"
QT_MOC_LITERAL(97, 2267, 19), // "onActionModifyStyle"
QT_MOC_LITERAL(98, 2287, 23), // "onActionModifySpringing"
QT_MOC_LITERAL(99, 2311, 21), // "onActionPatternSelect"
QT_MOC_LITERAL(100, 2333, 18), // "onActionPatternNew"
QT_MOC_LITERAL(101, 2352, 20), // "onActionPatternClone"
QT_MOC_LITERAL(102, 2373, 21), // "onActionPatternDelete"
QT_MOC_LITERAL(103, 2395, 21), // "onActionPatternRotate"
QT_MOC_LITERAL(104, 2417, 27), // "onActionPatternMakeSymmetic"
QT_MOC_LITERAL(105, 2445, 23), // "onActionPatternPosition"
QT_MOC_LITERAL(106, 2469, 26), // "onActionPatternExtractAuto"
QT_MOC_LITERAL(107, 2496, 28), // "onActionPatternExtractManual"
QT_MOC_LITERAL(108, 2525, 24), // "onActionPatternPerimeter"
QT_MOC_LITERAL(109, 2550, 19), // "onActionPatternTrim"
QT_MOC_LITERAL(110, 2570, 20), // "onActionPatternCheck"
QT_MOC_LITERAL(111, 2591, 21), // "onActionPatternDevide"
QT_MOC_LITERAL(112, 2613, 25), // "onActionPatternSaveAsWeft"
QT_MOC_LITERAL(113, 2639, 18), // "onActionViewXYAxes"
QT_MOC_LITERAL(114, 2658, 7), // "toggled"
QT_MOC_LITERAL(115, 2666, 16), // "onActionViewGrid"
QT_MOC_LITERAL(116, 2683, 17), // "onActionViewKnots"
QT_MOC_LITERAL(117, 2701, 17), // "onActionViewImage"
QT_MOC_LITERAL(118, 2719, 23), // "onActionViewDesignRules"
QT_MOC_LITERAL(119, 2743, 24), // "onActionViewGradingRules"
QT_MOC_LITERAL(120, 2768, 26), // "onActionViewFilledPatterns"
QT_MOC_LITERAL(121, 2795, 22), // "onActionViewZoomWindow"
QT_MOC_LITERAL(122, 2818, 19), // "onActionViewZoomAll"
QT_MOC_LITERAL(123, 2838, 18), // "onActionViewZoomIn"
QT_MOC_LITERAL(124, 2857, 19), // "onActionViewZoomOut"
QT_MOC_LITERAL(125, 2877, 20), // "onActionViewZoomBack"
QT_MOC_LITERAL(126, 2898, 27), // "onActionViewVoerlapPatterns"
QT_MOC_LITERAL(127, 2926, 16), // "onActionViewHide"
QT_MOC_LITERAL(128, 2943, 16), // "onActionViewShow"
QT_MOC_LITERAL(129, 2960, 17), // "onActionViewClean"
QT_MOC_LITERAL(130, 2978, 24), // "onActionViewSetReference"
QT_MOC_LITERAL(131, 3003, 22), // "onActionViewLockLayout"
QT_MOC_LITERAL(132, 3026, 32), // "onActionViewToolFindStyleToggled"
QT_MOC_LITERAL(133, 3059, 30), // "onActionViewToolProjectToggled"
QT_MOC_LITERAL(134, 3090, 33), // "onActionViewToolPropertiesTog..."
QT_MOC_LITERAL(135, 3124, 28), // "onActionViewToolSlideToggled"
QT_MOC_LITERAL(136, 3153, 32), // "onActionViewToolCustomizeToggled"
QT_MOC_LITERAL(137, 3186, 21), // "onActionViewStatusBar"
QT_MOC_LITERAL(138, 3208, 25), // "onActionUtilityMeasureDis"
QT_MOC_LITERAL(139, 3234, 26), // "onActionUtilityMeasureLine"
QT_MOC_LITERAL(140, 3261, 26), // "onActionCreatorPatternCard"
QT_MOC_LITERAL(141, 3288, 27), // "onActionUtilityQuickNesting"
QT_MOC_LITERAL(142, 3316, 28), // "onActionUtilityNestByJobCard"
QT_MOC_LITERAL(143, 3345, 24), // "onActionUtilityHeelCover"
QT_MOC_LITERAL(144, 3370, 28), // "onActionUtilityPatternSearch"
QT_MOC_LITERAL(145, 3399, 25), // "onActionGradingParameters"
QT_MOC_LITERAL(146, 3425, 20), // "onActionGradingGrade"
QT_MOC_LITERAL(147, 3446, 36), // "onActionGradingDeletedGradedP..."
QT_MOC_LITERAL(148, 3483, 30), // "onActionGradingReferenceInsert"
QT_MOC_LITERAL(149, 3514, 25), // "onActionGradingKeepMargin"
QT_MOC_LITERAL(150, 3540, 27), // "onActionGradingDeleteMargin"
QT_MOC_LITERAL(151, 3568, 21), // "onActionGradingUpdate"
QT_MOC_LITERAL(152, 3590, 21), // "onActionGradingXYLock"
QT_MOC_LITERAL(153, 3612, 27), // "onActionGradingDeleteXYLock"
QT_MOC_LITERAL(154, 3640, 29), // "onActionGradingHorizontalZone"
QT_MOC_LITERAL(155, 3670, 27), // "onActionGradingVerticalZone"
QT_MOC_LITERAL(156, 3698, 25), // "onActionGradingDeleteZone"
QT_MOC_LITERAL(157, 3724, 16), // "onActionHelpHelp"
QT_MOC_LITERAL(158, 3741, 19), // "onActionHelpLicense"
QT_MOC_LITERAL(159, 3761, 17), // "onActionHelpAbout"
QT_MOC_LITERAL(160, 3779, 26), // "onActionObjectSnapEndpoint"
QT_MOC_LITERAL(161, 3806, 24), // "onActionObjectSnapCenter"
QT_MOC_LITERAL(162, 3831, 30), // "onActionObjectSnapInterSection"
QT_MOC_LITERAL(163, 3862, 22), // "onActionObjectSnapKnot"
QT_MOC_LITERAL(164, 3885, 23), // "onActionObjectSnapOrtho"
QT_MOC_LITERAL(165, 3909, 34), // "onActionObjectSnapEnterCoordi..."
QT_MOC_LITERAL(166, 3944, 18), // "onToolSlideChanged"
QT_MOC_LITERAL(167, 3963, 32), // "onDockFindStyleVisibilityChanged"
QT_MOC_LITERAL(168, 3996, 7), // "visible"
QT_MOC_LITERAL(169, 4004, 30), // "onDockProjectVisibilityChanged"
QT_MOC_LITERAL(170, 4035, 33), // "onDockPropertiesVisibilityCha..."
QT_MOC_LITERAL(171, 4069, 20), // "slideFindStyleToggle"
QT_MOC_LITERAL(172, 4090, 18), // "slideProjectToggle"
QT_MOC_LITERAL(173, 4109, 21), // "slidePropertiesToggle"
QT_MOC_LITERAL(174, 4131, 21), // "onActionTreeExpandAll"
QT_MOC_LITERAL(175, 4153, 19), // "onActionTreeFoldAll"
QT_MOC_LITERAL(176, 4173, 30), // "onTreeProjectItemDoubleClicked"
QT_MOC_LITERAL(177, 4204, 16), // "QTreeWidgetItem*"
QT_MOC_LITERAL(178, 4221, 4), // "item"
QT_MOC_LITERAL(179, 4226, 27), // "onActionTreeProjectAddScene"
QT_MOC_LITERAL(180, 4254, 23), // "onActionTreeProjectSave"
QT_MOC_LITERAL(181, 4278, 25), // "onActionTreeProjectSaveAs"
QT_MOC_LITERAL(182, 4304, 25), // "onActionTreeProjectRename"
QT_MOC_LITERAL(183, 4330, 24), // "onActionTreeProjectClose"
QT_MOC_LITERAL(184, 4355, 32), // "onActionTreeProjectSceneChangeTo"
QT_MOC_LITERAL(185, 4388, 33), // "onActionTreeProjectSceneMoveU..."
QT_MOC_LITERAL(186, 4422, 33), // "onActionTreeProjectSceneMoveU..."
QT_MOC_LITERAL(187, 4456, 35), // "onActionTreeProjectSceneMoveD..."
QT_MOC_LITERAL(188, 4492, 38), // "onActionTreeProjectSceneMoveD..."
QT_MOC_LITERAL(189, 4531, 30), // "onActionTreeProjectSceneRename"
QT_MOC_LITERAL(190, 4562, 30), // "onActionTreeProjectSceneDelete"
QT_MOC_LITERAL(191, 4593, 14), // "updateViewMenu"
QT_MOC_LITERAL(192, 4608, 20), // "onProjectNameChanged"
QT_MOC_LITERAL(193, 4629, 8), // "lastName"
QT_MOC_LITERAL(194, 4638, 11), // "presentName"
QT_MOC_LITERAL(195, 4650, 22), // "onMousePositionChanged"
QT_MOC_LITERAL(196, 4673, 3), // "pos"
QT_MOC_LITERAL(197, 4677, 19), // "onSceneItemsChanged"
QT_MOC_LITERAL(198, 4697, 15), // "onPointSelected"
QT_MOC_LITERAL(199, 4713, 6), // "Point*"
QT_MOC_LITERAL(200, 4720, 5), // "point"
QT_MOC_LITERAL(201, 4726, 14), // "onLineSelected"
QT_MOC_LITERAL(202, 4741, 5), // "Line*"
QT_MOC_LITERAL(203, 4747, 4), // "line"
QT_MOC_LITERAL(204, 4752, 13), // "onArcSelected"
QT_MOC_LITERAL(205, 4766, 4), // "Arc*"
QT_MOC_LITERAL(206, 4771, 3), // "arc"
QT_MOC_LITERAL(207, 4775, 17), // "onEllipseSelected"
QT_MOC_LITERAL(208, 4793, 8), // "Ellipse*"
QT_MOC_LITERAL(209, 4802, 7), // "ellipse"
QT_MOC_LITERAL(210, 4810, 16), // "onCircleSelected"
QT_MOC_LITERAL(211, 4827, 7), // "Circle*"
QT_MOC_LITERAL(212, 4835, 6), // "circle"
QT_MOC_LITERAL(213, 4842, 14), // "onRectSelected"
QT_MOC_LITERAL(214, 4857, 5), // "Rect*"
QT_MOC_LITERAL(215, 4863, 4), // "rect"
QT_MOC_LITERAL(216, 4868, 18), // "onPolylineSelected"
QT_MOC_LITERAL(217, 4887, 9), // "Polyline*"
QT_MOC_LITERAL(218, 4897, 8) // "polyline"

    },
    "Sketch\0configChanged\0\0name\0value\0"
    "onActionFileNew\0onActionFileOpen\0"
    "onActionFileSave\0onActionFileSaveAs\0"
    "onActionFileSaveAll\0onActionFilePrint\0"
    "onActionFilePrintPreview\0"
    "onActionFilePrintSetup\0onActionFileImportDXF\0"
    "onActionFileImportDVS\0onActionFileImportTEF\0"
    "onActionFileExportDXF\0onActionFileExportDVS\0"
    "onActionFileExportTEF\0onActionFileConfiguration\0"
    "onActionFileExit\0onActionDrawLine\0"
    "onActionDrawEllipse\0onActionDrawRect\0"
    "onActionDrawMiddleAxis\0onActionDrawPolyline\0"
    "onActionDrawCircle\0onActionDrawArcBy3Pnts\0"
    "onActionDrawArcBy3Pnts2\0onActionDrawTrapezium\0"
    "onActionDrawPolygon\0onActionDrawStabHole\0"
    "onActionDrawReference\0onActionDrawEyelet\0"
    "onActionDrawPatternDirection\0"
    "onActionDrawShankLine\0onActionDrawPerpendicular\0"
    "onActionDrawImage\0onActionDrawImageInsert\0"
    "onActionDrawImageDelete\0onActionDrawImageCrop\0"
    "onActionDrawImageCalibrate\0"
    "onActionDrawImageAdjustment\0"
    "onActionDrawDigitizerActivate\0"
    "onActionDrawDigitizerSetPosition\0"
    "onActionDrawDigitizerConfigue\0"
    "onActionDrawScannerVectorizeFromScanner\0"
    "onActionDrawScannerSelectSource\0"
    "onActionDrawScannerVectorizeImage\0"
    "onActionInsertOffset\0onActionInsertAdvancedOffset\0"
    "onActionInsertText\0onActionInsertFillet\0"
    "onActionInsertNotch\0onActionInsertNotchesGroup\0"
    "onActionInsertNotchesAxisTiedGroup\0"
    "onActionInsertSizeNotch\0onActionInsertRemark\0"
    "onActionInsertSmooth\0onActionInsertMargin\0"
    "onActionInsertCanel\0onActionInsertBlock\0"
    "onActionInsertHolesOverALine\0"
    "onActionInsertArrayRectangle\0"
    "onActionInsertArrayCircular\0"
    "onActionModifyUndo\0onActionModifyRedo\0"
    "onActionModifyClear\0onActionModifyDelete\0"
    "onActionModifyCut\0onActionModifyCopy\0"
    "onActionModifyPaste\0onActionModifyPasteAsSlave\0"
    "onActionModifyTransformMove\0"
    "onActionModifyTransformRotate\0"
    "onActionModifyTransformRotateKnotsContinuely\0"
    "onActionModifyTransformRotateKnots\0"
    "onActionModifyTransformScale\0"
    "onActionModifyTransformMirror\0"
    "onActionModifyTransformStrech\0"
    "onActionModifyTransformExplode\0"
    "onActionModifyTransformJoin\0"
    "onActionModifyTransformBreak\0"
    "onActionModifyTransformfillet\0"
    "onActionModifyTransformVCut\0"
    "onActionModifyTransformSpline\0"
    "onActionModifyTrim\0onActionModifyExtend\0"
    "onActionModifyEndpoint\0"
    "onActionModifyAdjustPolyline\0"
    "onActionModifyStartpoint\0"
    "onActionModifySoftenup\0onActionModifyWarp\0"
    "onActionModifyKnotsReduction\0"
    "onActionModifyReplacePart\0"
    "onActionModifyChangeTo\0"
    "onActionModifyMakeMasterLine\0"
    "onActionModifyStyle\0onActionModifySpringing\0"
    "onActionPatternSelect\0onActionPatternNew\0"
    "onActionPatternClone\0onActionPatternDelete\0"
    "onActionPatternRotate\0onActionPatternMakeSymmetic\0"
    "onActionPatternPosition\0"
    "onActionPatternExtractAuto\0"
    "onActionPatternExtractManual\0"
    "onActionPatternPerimeter\0onActionPatternTrim\0"
    "onActionPatternCheck\0onActionPatternDevide\0"
    "onActionPatternSaveAsWeft\0onActionViewXYAxes\0"
    "toggled\0onActionViewGrid\0onActionViewKnots\0"
    "onActionViewImage\0onActionViewDesignRules\0"
    "onActionViewGradingRules\0"
    "onActionViewFilledPatterns\0"
    "onActionViewZoomWindow\0onActionViewZoomAll\0"
    "onActionViewZoomIn\0onActionViewZoomOut\0"
    "onActionViewZoomBack\0onActionViewVoerlapPatterns\0"
    "onActionViewHide\0onActionViewShow\0"
    "onActionViewClean\0onActionViewSetReference\0"
    "onActionViewLockLayout\0"
    "onActionViewToolFindStyleToggled\0"
    "onActionViewToolProjectToggled\0"
    "onActionViewToolPropertiesToggled\0"
    "onActionViewToolSlideToggled\0"
    "onActionViewToolCustomizeToggled\0"
    "onActionViewStatusBar\0onActionUtilityMeasureDis\0"
    "onActionUtilityMeasureLine\0"
    "onActionCreatorPatternCard\0"
    "onActionUtilityQuickNesting\0"
    "onActionUtilityNestByJobCard\0"
    "onActionUtilityHeelCover\0"
    "onActionUtilityPatternSearch\0"
    "onActionGradingParameters\0"
    "onActionGradingGrade\0"
    "onActionGradingDeletedGradedPatterns\0"
    "onActionGradingReferenceInsert\0"
    "onActionGradingKeepMargin\0"
    "onActionGradingDeleteMargin\0"
    "onActionGradingUpdate\0onActionGradingXYLock\0"
    "onActionGradingDeleteXYLock\0"
    "onActionGradingHorizontalZone\0"
    "onActionGradingVerticalZone\0"
    "onActionGradingDeleteZone\0onActionHelpHelp\0"
    "onActionHelpLicense\0onActionHelpAbout\0"
    "onActionObjectSnapEndpoint\0"
    "onActionObjectSnapCenter\0"
    "onActionObjectSnapInterSection\0"
    "onActionObjectSnapKnot\0onActionObjectSnapOrtho\0"
    "onActionObjectSnapEnterCoordinates\0"
    "onToolSlideChanged\0onDockFindStyleVisibilityChanged\0"
    "visible\0onDockProjectVisibilityChanged\0"
    "onDockPropertiesVisibilityChanged\0"
    "slideFindStyleToggle\0slideProjectToggle\0"
    "slidePropertiesToggle\0onActionTreeExpandAll\0"
    "onActionTreeFoldAll\0onTreeProjectItemDoubleClicked\0"
    "QTreeWidgetItem*\0item\0onActionTreeProjectAddScene\0"
    "onActionTreeProjectSave\0"
    "onActionTreeProjectSaveAs\0"
    "onActionTreeProjectRename\0"
    "onActionTreeProjectClose\0"
    "onActionTreeProjectSceneChangeTo\0"
    "onActionTreeProjectSceneMoveUpOne\0"
    "onActionTreeProjectSceneMoveUpTop\0"
    "onActionTreeProjectSceneMoveDownOne\0"
    "onActionTreeProjectSceneMoveDownBottom\0"
    "onActionTreeProjectSceneRename\0"
    "onActionTreeProjectSceneDelete\0"
    "updateViewMenu\0onProjectNameChanged\0"
    "lastName\0presentName\0onMousePositionChanged\0"
    "pos\0onSceneItemsChanged\0onPointSelected\0"
    "Point*\0point\0onLineSelected\0Line*\0"
    "line\0onArcSelected\0Arc*\0arc\0"
    "onEllipseSelected\0Ellipse*\0ellipse\0"
    "onCircleSelected\0Circle*\0circle\0"
    "onRectSelected\0Rect*\0rect\0onPolylineSelected\0"
    "Polyline*\0polyline"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Sketch[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
     194,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,  984,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,  989,    2, 0x08 /* Private */,
       6,    0,  990,    2, 0x08 /* Private */,
       7,    0,  991,    2, 0x08 /* Private */,
       8,    0,  992,    2, 0x08 /* Private */,
       9,    0,  993,    2, 0x08 /* Private */,
      10,    0,  994,    2, 0x08 /* Private */,
      11,    0,  995,    2, 0x08 /* Private */,
      12,    0,  996,    2, 0x08 /* Private */,
      13,    0,  997,    2, 0x08 /* Private */,
      14,    0,  998,    2, 0x08 /* Private */,
      15,    0,  999,    2, 0x08 /* Private */,
      16,    0, 1000,    2, 0x08 /* Private */,
      17,    0, 1001,    2, 0x08 /* Private */,
      18,    0, 1002,    2, 0x08 /* Private */,
      19,    0, 1003,    2, 0x08 /* Private */,
      20,    0, 1004,    2, 0x08 /* Private */,
      21,    0, 1005,    2, 0x08 /* Private */,
      22,    0, 1006,    2, 0x08 /* Private */,
      23,    0, 1007,    2, 0x08 /* Private */,
      24,    0, 1008,    2, 0x08 /* Private */,
      25,    0, 1009,    2, 0x08 /* Private */,
      26,    0, 1010,    2, 0x08 /* Private */,
      27,    0, 1011,    2, 0x08 /* Private */,
      28,    0, 1012,    2, 0x08 /* Private */,
      29,    0, 1013,    2, 0x08 /* Private */,
      30,    0, 1014,    2, 0x08 /* Private */,
      31,    0, 1015,    2, 0x08 /* Private */,
      32,    0, 1016,    2, 0x08 /* Private */,
      33,    0, 1017,    2, 0x08 /* Private */,
      34,    0, 1018,    2, 0x08 /* Private */,
      35,    0, 1019,    2, 0x08 /* Private */,
      36,    0, 1020,    2, 0x08 /* Private */,
      37,    0, 1021,    2, 0x08 /* Private */,
      38,    0, 1022,    2, 0x08 /* Private */,
      39,    0, 1023,    2, 0x08 /* Private */,
      40,    0, 1024,    2, 0x08 /* Private */,
      41,    0, 1025,    2, 0x08 /* Private */,
      42,    0, 1026,    2, 0x08 /* Private */,
      43,    0, 1027,    2, 0x08 /* Private */,
      44,    0, 1028,    2, 0x08 /* Private */,
      45,    0, 1029,    2, 0x08 /* Private */,
      46,    0, 1030,    2, 0x08 /* Private */,
      47,    0, 1031,    2, 0x08 /* Private */,
      48,    0, 1032,    2, 0x08 /* Private */,
      49,    0, 1033,    2, 0x08 /* Private */,
      50,    0, 1034,    2, 0x08 /* Private */,
      51,    0, 1035,    2, 0x08 /* Private */,
      52,    0, 1036,    2, 0x08 /* Private */,
      53,    0, 1037,    2, 0x08 /* Private */,
      54,    0, 1038,    2, 0x08 /* Private */,
      55,    0, 1039,    2, 0x08 /* Private */,
      56,    0, 1040,    2, 0x08 /* Private */,
      57,    0, 1041,    2, 0x08 /* Private */,
      58,    0, 1042,    2, 0x08 /* Private */,
      59,    0, 1043,    2, 0x08 /* Private */,
      60,    0, 1044,    2, 0x08 /* Private */,
      61,    0, 1045,    2, 0x08 /* Private */,
      62,    0, 1046,    2, 0x08 /* Private */,
      63,    0, 1047,    2, 0x08 /* Private */,
      64,    0, 1048,    2, 0x08 /* Private */,
      65,    0, 1049,    2, 0x08 /* Private */,
      66,    0, 1050,    2, 0x08 /* Private */,
      67,    0, 1051,    2, 0x08 /* Private */,
      68,    0, 1052,    2, 0x08 /* Private */,
      69,    0, 1053,    2, 0x08 /* Private */,
      70,    0, 1054,    2, 0x08 /* Private */,
      71,    0, 1055,    2, 0x08 /* Private */,
      72,    0, 1056,    2, 0x08 /* Private */,
      73,    0, 1057,    2, 0x08 /* Private */,
      74,    0, 1058,    2, 0x08 /* Private */,
      75,    0, 1059,    2, 0x08 /* Private */,
      76,    0, 1060,    2, 0x08 /* Private */,
      77,    0, 1061,    2, 0x08 /* Private */,
      78,    0, 1062,    2, 0x08 /* Private */,
      79,    0, 1063,    2, 0x08 /* Private */,
      80,    0, 1064,    2, 0x08 /* Private */,
      81,    0, 1065,    2, 0x08 /* Private */,
      82,    0, 1066,    2, 0x08 /* Private */,
      83,    0, 1067,    2, 0x08 /* Private */,
      84,    0, 1068,    2, 0x08 /* Private */,
      85,    0, 1069,    2, 0x08 /* Private */,
      86,    0, 1070,    2, 0x08 /* Private */,
      87,    0, 1071,    2, 0x08 /* Private */,
      88,    0, 1072,    2, 0x08 /* Private */,
      89,    0, 1073,    2, 0x08 /* Private */,
      90,    0, 1074,    2, 0x08 /* Private */,
      91,    0, 1075,    2, 0x08 /* Private */,
      92,    0, 1076,    2, 0x08 /* Private */,
      93,    0, 1077,    2, 0x08 /* Private */,
      94,    0, 1078,    2, 0x08 /* Private */,
      95,    0, 1079,    2, 0x08 /* Private */,
      96,    0, 1080,    2, 0x08 /* Private */,
      97,    0, 1081,    2, 0x08 /* Private */,
      98,    0, 1082,    2, 0x08 /* Private */,
      99,    0, 1083,    2, 0x08 /* Private */,
     100,    0, 1084,    2, 0x08 /* Private */,
     101,    0, 1085,    2, 0x08 /* Private */,
     102,    0, 1086,    2, 0x08 /* Private */,
     103,    0, 1087,    2, 0x08 /* Private */,
     104,    0, 1088,    2, 0x08 /* Private */,
     105,    0, 1089,    2, 0x08 /* Private */,
     106,    0, 1090,    2, 0x08 /* Private */,
     107,    0, 1091,    2, 0x08 /* Private */,
     108,    0, 1092,    2, 0x08 /* Private */,
     109,    0, 1093,    2, 0x08 /* Private */,
     110,    0, 1094,    2, 0x08 /* Private */,
     111,    0, 1095,    2, 0x08 /* Private */,
     112,    0, 1096,    2, 0x08 /* Private */,
     113,    1, 1097,    2, 0x08 /* Private */,
     115,    1, 1100,    2, 0x08 /* Private */,
     116,    1, 1103,    2, 0x08 /* Private */,
     117,    1, 1106,    2, 0x08 /* Private */,
     118,    1, 1109,    2, 0x08 /* Private */,
     119,    1, 1112,    2, 0x08 /* Private */,
     120,    1, 1115,    2, 0x08 /* Private */,
     121,    0, 1118,    2, 0x08 /* Private */,
     122,    0, 1119,    2, 0x08 /* Private */,
     123,    0, 1120,    2, 0x08 /* Private */,
     124,    0, 1121,    2, 0x08 /* Private */,
     125,    0, 1122,    2, 0x08 /* Private */,
     126,    0, 1123,    2, 0x08 /* Private */,
     127,    1, 1124,    2, 0x08 /* Private */,
     128,    1, 1127,    2, 0x08 /* Private */,
     129,    1, 1130,    2, 0x08 /* Private */,
     130,    0, 1133,    2, 0x08 /* Private */,
     131,    1, 1134,    2, 0x08 /* Private */,
     132,    1, 1137,    2, 0x08 /* Private */,
     133,    1, 1140,    2, 0x08 /* Private */,
     134,    1, 1143,    2, 0x08 /* Private */,
     135,    1, 1146,    2, 0x08 /* Private */,
     136,    1, 1149,    2, 0x08 /* Private */,
     137,    1, 1152,    2, 0x08 /* Private */,
     138,    0, 1155,    2, 0x08 /* Private */,
     139,    0, 1156,    2, 0x08 /* Private */,
     140,    0, 1157,    2, 0x08 /* Private */,
     141,    0, 1158,    2, 0x08 /* Private */,
     142,    0, 1159,    2, 0x08 /* Private */,
     143,    0, 1160,    2, 0x08 /* Private */,
     144,    0, 1161,    2, 0x08 /* Private */,
     145,    0, 1162,    2, 0x08 /* Private */,
     146,    0, 1163,    2, 0x08 /* Private */,
     147,    0, 1164,    2, 0x08 /* Private */,
     148,    0, 1165,    2, 0x08 /* Private */,
     149,    0, 1166,    2, 0x08 /* Private */,
     150,    0, 1167,    2, 0x08 /* Private */,
     151,    0, 1168,    2, 0x08 /* Private */,
     152,    0, 1169,    2, 0x08 /* Private */,
     153,    0, 1170,    2, 0x08 /* Private */,
     154,    0, 1171,    2, 0x08 /* Private */,
     155,    0, 1172,    2, 0x08 /* Private */,
     156,    0, 1173,    2, 0x08 /* Private */,
     157,    0, 1174,    2, 0x08 /* Private */,
     158,    0, 1175,    2, 0x08 /* Private */,
     159,    0, 1176,    2, 0x08 /* Private */,
     160,    0, 1177,    2, 0x08 /* Private */,
     161,    0, 1178,    2, 0x08 /* Private */,
     162,    0, 1179,    2, 0x08 /* Private */,
     163,    0, 1180,    2, 0x08 /* Private */,
     164,    0, 1181,    2, 0x08 /* Private */,
     165,    0, 1182,    2, 0x08 /* Private */,
     166,    0, 1183,    2, 0x08 /* Private */,
     167,    1, 1184,    2, 0x08 /* Private */,
     169,    1, 1187,    2, 0x08 /* Private */,
     170,    1, 1190,    2, 0x08 /* Private */,
     171,    0, 1193,    2, 0x08 /* Private */,
     172,    0, 1194,    2, 0x08 /* Private */,
     173,    0, 1195,    2, 0x08 /* Private */,
     174,    0, 1196,    2, 0x08 /* Private */,
     175,    0, 1197,    2, 0x08 /* Private */,
     176,    1, 1198,    2, 0x08 /* Private */,
     179,    0, 1201,    2, 0x08 /* Private */,
     180,    0, 1202,    2, 0x08 /* Private */,
     181,    0, 1203,    2, 0x08 /* Private */,
     182,    0, 1204,    2, 0x08 /* Private */,
     183,    0, 1205,    2, 0x08 /* Private */,
     184,    0, 1206,    2, 0x08 /* Private */,
     185,    0, 1207,    2, 0x08 /* Private */,
     186,    0, 1208,    2, 0x08 /* Private */,
     187,    0, 1209,    2, 0x08 /* Private */,
     188,    0, 1210,    2, 0x08 /* Private */,
     189,    0, 1211,    2, 0x08 /* Private */,
     190,    0, 1212,    2, 0x08 /* Private */,
     191,    0, 1213,    2, 0x08 /* Private */,
     192,    2, 1214,    2, 0x0a /* Public */,
     195,    1, 1219,    2, 0x0a /* Public */,
     197,    0, 1222,    2, 0x0a /* Public */,
     198,    1, 1223,    2, 0x0a /* Public */,
     201,    1, 1226,    2, 0x0a /* Public */,
     204,    1, 1229,    2, 0x0a /* Public */,
     207,    1, 1232,    2, 0x0a /* Public */,
     210,    1, 1235,    2, 0x0a /* Public */,
     213,    1, 1238,    2, 0x0a /* Public */,
     216,    1, 1241,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QVariant,    3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool,
    QMetaType::Bool,
    QMetaType::Bool,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,  114,
    QMetaType::Void, QMetaType::Bool,  114,
    QMetaType::Void, QMetaType::Bool,  114,
    QMetaType::Void, QMetaType::Bool,  114,
    QMetaType::Void, QMetaType::Bool,  114,
    QMetaType::Void, QMetaType::Bool,  114,
    QMetaType::Void, QMetaType::Bool,  114,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,  114,
    QMetaType::Void, QMetaType::Bool,  114,
    QMetaType::Void, QMetaType::Bool,  114,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,  114,
    QMetaType::Void, QMetaType::Bool,  114,
    QMetaType::Void, QMetaType::Bool,  114,
    QMetaType::Void, QMetaType::Bool,  114,
    QMetaType::Void, QMetaType::Bool,  114,
    QMetaType::Void, QMetaType::Bool,  114,
    QMetaType::Void, QMetaType::Bool,  114,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,  168,
    QMetaType::Void, QMetaType::Bool,  168,
    QMetaType::Void, QMetaType::Bool,  168,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 177,  178,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,  193,  194,
    QMetaType::Void, QMetaType::QPointF,  196,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 199,  200,
    QMetaType::Void, 0x80000000 | 202,  203,
    QMetaType::Void, 0x80000000 | 205,  206,
    QMetaType::Void, 0x80000000 | 208,  209,
    QMetaType::Void, 0x80000000 | 211,  212,
    QMetaType::Void, 0x80000000 | 214,  215,
    QMetaType::Void, 0x80000000 | 217,  218,

       0        // eod
};

void Sketch::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Sketch *_t = static_cast<Sketch *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->configChanged((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QVariant(*)>(_a[2]))); break;
        case 1: _t->onActionFileNew(); break;
        case 2: _t->onActionFileOpen(); break;
        case 3: { bool _r = _t->onActionFileSave();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 4: { bool _r = _t->onActionFileSaveAs();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 5: { bool _r = _t->onActionFileSaveAll();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 6: _t->onActionFilePrint(); break;
        case 7: _t->onActionFilePrintPreview(); break;
        case 8: _t->onActionFilePrintSetup(); break;
        case 9: _t->onActionFileImportDXF(); break;
        case 10: _t->onActionFileImportDVS(); break;
        case 11: _t->onActionFileImportTEF(); break;
        case 12: _t->onActionFileExportDXF(); break;
        case 13: _t->onActionFileExportDVS(); break;
        case 14: _t->onActionFileExportTEF(); break;
        case 15: _t->onActionFileConfiguration(); break;
        case 16: _t->onActionFileExit(); break;
        case 17: _t->onActionDrawLine(); break;
        case 18: _t->onActionDrawEllipse(); break;
        case 19: _t->onActionDrawRect(); break;
        case 20: _t->onActionDrawMiddleAxis(); break;
        case 21: _t->onActionDrawPolyline(); break;
        case 22: _t->onActionDrawCircle(); break;
        case 23: _t->onActionDrawArcBy3Pnts(); break;
        case 24: _t->onActionDrawArcBy3Pnts2(); break;
        case 25: _t->onActionDrawTrapezium(); break;
        case 26: _t->onActionDrawPolygon(); break;
        case 27: _t->onActionDrawStabHole(); break;
        case 28: _t->onActionDrawReference(); break;
        case 29: _t->onActionDrawEyelet(); break;
        case 30: _t->onActionDrawPatternDirection(); break;
        case 31: _t->onActionDrawShankLine(); break;
        case 32: _t->onActionDrawPerpendicular(); break;
        case 33: _t->onActionDrawImage(); break;
        case 34: _t->onActionDrawImageInsert(); break;
        case 35: _t->onActionDrawImageDelete(); break;
        case 36: _t->onActionDrawImageCrop(); break;
        case 37: _t->onActionDrawImageCalibrate(); break;
        case 38: _t->onActionDrawImageAdjustment(); break;
        case 39: _t->onActionDrawDigitizerActivate(); break;
        case 40: _t->onActionDrawDigitizerSetPosition(); break;
        case 41: _t->onActionDrawDigitizerConfigue(); break;
        case 42: _t->onActionDrawScannerVectorizeFromScanner(); break;
        case 43: _t->onActionDrawScannerSelectSource(); break;
        case 44: _t->onActionDrawScannerVectorizeImage(); break;
        case 45: _t->onActionInsertOffset(); break;
        case 46: _t->onActionInsertAdvancedOffset(); break;
        case 47: _t->onActionInsertText(); break;
        case 48: _t->onActionInsertFillet(); break;
        case 49: _t->onActionInsertNotch(); break;
        case 50: _t->onActionInsertNotchesGroup(); break;
        case 51: _t->onActionInsertNotchesAxisTiedGroup(); break;
        case 52: _t->onActionInsertSizeNotch(); break;
        case 53: _t->onActionInsertRemark(); break;
        case 54: _t->onActionInsertSmooth(); break;
        case 55: _t->onActionInsertMargin(); break;
        case 56: _t->onActionInsertCanel(); break;
        case 57: _t->onActionInsertBlock(); break;
        case 58: _t->onActionInsertHolesOverALine(); break;
        case 59: _t->onActionInsertArrayRectangle(); break;
        case 60: _t->onActionInsertArrayCircular(); break;
        case 61: _t->onActionModifyUndo(); break;
        case 62: _t->onActionModifyRedo(); break;
        case 63: _t->onActionModifyClear(); break;
        case 64: _t->onActionModifyDelete(); break;
        case 65: _t->onActionModifyCut(); break;
        case 66: _t->onActionModifyCopy(); break;
        case 67: _t->onActionModifyPaste(); break;
        case 68: _t->onActionModifyPasteAsSlave(); break;
        case 69: _t->onActionModifyTransformMove(); break;
        case 70: _t->onActionModifyTransformRotate(); break;
        case 71: _t->onActionModifyTransformRotateKnotsContinuely(); break;
        case 72: _t->onActionModifyTransformRotateKnots(); break;
        case 73: _t->onActionModifyTransformScale(); break;
        case 74: _t->onActionModifyTransformMirror(); break;
        case 75: _t->onActionModifyTransformStrech(); break;
        case 76: _t->onActionModifyTransformExplode(); break;
        case 77: _t->onActionModifyTransformJoin(); break;
        case 78: _t->onActionModifyTransformBreak(); break;
        case 79: _t->onActionModifyTransformfillet(); break;
        case 80: _t->onActionModifyTransformVCut(); break;
        case 81: _t->onActionModifyTransformSpline(); break;
        case 82: _t->onActionModifyTrim(); break;
        case 83: _t->onActionModifyExtend(); break;
        case 84: _t->onActionModifyEndpoint(); break;
        case 85: _t->onActionModifyAdjustPolyline(); break;
        case 86: _t->onActionModifyStartpoint(); break;
        case 87: _t->onActionModifySoftenup(); break;
        case 88: _t->onActionModifyWarp(); break;
        case 89: _t->onActionModifyKnotsReduction(); break;
        case 90: _t->onActionModifyReplacePart(); break;
        case 91: _t->onActionModifyChangeTo(); break;
        case 92: _t->onActionModifyMakeMasterLine(); break;
        case 93: _t->onActionModifyStyle(); break;
        case 94: _t->onActionModifySpringing(); break;
        case 95: _t->onActionPatternSelect(); break;
        case 96: _t->onActionPatternNew(); break;
        case 97: _t->onActionPatternClone(); break;
        case 98: _t->onActionPatternDelete(); break;
        case 99: _t->onActionPatternRotate(); break;
        case 100: _t->onActionPatternMakeSymmetic(); break;
        case 101: _t->onActionPatternPosition(); break;
        case 102: _t->onActionPatternExtractAuto(); break;
        case 103: _t->onActionPatternExtractManual(); break;
        case 104: _t->onActionPatternPerimeter(); break;
        case 105: _t->onActionPatternTrim(); break;
        case 106: _t->onActionPatternCheck(); break;
        case 107: _t->onActionPatternDevide(); break;
        case 108: _t->onActionPatternSaveAsWeft(); break;
        case 109: _t->onActionViewXYAxes((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 110: _t->onActionViewGrid((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 111: _t->onActionViewKnots((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 112: _t->onActionViewImage((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 113: _t->onActionViewDesignRules((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 114: _t->onActionViewGradingRules((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 115: _t->onActionViewFilledPatterns((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 116: _t->onActionViewZoomWindow(); break;
        case 117: _t->onActionViewZoomAll(); break;
        case 118: _t->onActionViewZoomIn(); break;
        case 119: _t->onActionViewZoomOut(); break;
        case 120: _t->onActionViewZoomBack(); break;
        case 121: _t->onActionViewVoerlapPatterns(); break;
        case 122: _t->onActionViewHide((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 123: _t->onActionViewShow((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 124: _t->onActionViewClean((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 125: _t->onActionViewSetReference(); break;
        case 126: _t->onActionViewLockLayout((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 127: _t->onActionViewToolFindStyleToggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 128: _t->onActionViewToolProjectToggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 129: _t->onActionViewToolPropertiesToggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 130: _t->onActionViewToolSlideToggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 131: _t->onActionViewToolCustomizeToggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 132: _t->onActionViewStatusBar((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 133: _t->onActionUtilityMeasureDis(); break;
        case 134: _t->onActionUtilityMeasureLine(); break;
        case 135: _t->onActionCreatorPatternCard(); break;
        case 136: _t->onActionUtilityQuickNesting(); break;
        case 137: _t->onActionUtilityNestByJobCard(); break;
        case 138: _t->onActionUtilityHeelCover(); break;
        case 139: _t->onActionUtilityPatternSearch(); break;
        case 140: _t->onActionGradingParameters(); break;
        case 141: _t->onActionGradingGrade(); break;
        case 142: _t->onActionGradingDeletedGradedPatterns(); break;
        case 143: _t->onActionGradingReferenceInsert(); break;
        case 144: _t->onActionGradingKeepMargin(); break;
        case 145: _t->onActionGradingDeleteMargin(); break;
        case 146: _t->onActionGradingUpdate(); break;
        case 147: _t->onActionGradingXYLock(); break;
        case 148: _t->onActionGradingDeleteXYLock(); break;
        case 149: _t->onActionGradingHorizontalZone(); break;
        case 150: _t->onActionGradingVerticalZone(); break;
        case 151: _t->onActionGradingDeleteZone(); break;
        case 152: _t->onActionHelpHelp(); break;
        case 153: _t->onActionHelpLicense(); break;
        case 154: _t->onActionHelpAbout(); break;
        case 155: _t->onActionObjectSnapEndpoint(); break;
        case 156: _t->onActionObjectSnapCenter(); break;
        case 157: _t->onActionObjectSnapInterSection(); break;
        case 158: _t->onActionObjectSnapKnot(); break;
        case 159: _t->onActionObjectSnapOrtho(); break;
        case 160: _t->onActionObjectSnapEnterCoordinates(); break;
        case 161: _t->onToolSlideChanged(); break;
        case 162: _t->onDockFindStyleVisibilityChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 163: _t->onDockProjectVisibilityChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 164: _t->onDockPropertiesVisibilityChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 165: _t->slideFindStyleToggle(); break;
        case 166: _t->slideProjectToggle(); break;
        case 167: _t->slidePropertiesToggle(); break;
        case 168: _t->onActionTreeExpandAll(); break;
        case 169: _t->onActionTreeFoldAll(); break;
        case 170: _t->onTreeProjectItemDoubleClicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 171: _t->onActionTreeProjectAddScene(); break;
        case 172: _t->onActionTreeProjectSave(); break;
        case 173: _t->onActionTreeProjectSaveAs(); break;
        case 174: _t->onActionTreeProjectRename(); break;
        case 175: _t->onActionTreeProjectClose(); break;
        case 176: _t->onActionTreeProjectSceneChangeTo(); break;
        case 177: _t->onActionTreeProjectSceneMoveUpOne(); break;
        case 178: _t->onActionTreeProjectSceneMoveUpTop(); break;
        case 179: _t->onActionTreeProjectSceneMoveDownOne(); break;
        case 180: _t->onActionTreeProjectSceneMoveDownBottom(); break;
        case 181: _t->onActionTreeProjectSceneRename(); break;
        case 182: _t->onActionTreeProjectSceneDelete(); break;
        case 183: _t->updateViewMenu(); break;
        case 184: _t->onProjectNameChanged((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 185: _t->onMousePositionChanged((*reinterpret_cast< QPointF(*)>(_a[1]))); break;
        case 186: _t->onSceneItemsChanged(); break;
        case 187: _t->onPointSelected((*reinterpret_cast< Point*(*)>(_a[1]))); break;
        case 188: _t->onLineSelected((*reinterpret_cast< Line*(*)>(_a[1]))); break;
        case 189: _t->onArcSelected((*reinterpret_cast< Arc*(*)>(_a[1]))); break;
        case 190: _t->onEllipseSelected((*reinterpret_cast< Ellipse*(*)>(_a[1]))); break;
        case 191: _t->onCircleSelected((*reinterpret_cast< Circle*(*)>(_a[1]))); break;
        case 192: _t->onRectSelected((*reinterpret_cast< Rect*(*)>(_a[1]))); break;
        case 193: _t->onPolylineSelected((*reinterpret_cast< Polyline*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 187:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Point* >(); break;
            }
            break;
        case 188:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Line* >(); break;
            }
            break;
        case 189:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Arc* >(); break;
            }
            break;
        case 190:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Ellipse* >(); break;
            }
            break;
        case 191:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Circle* >(); break;
            }
            break;
        case 192:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Rect* >(); break;
            }
            break;
        case 193:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Polyline* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (Sketch::*_t)(QString , QVariant );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Sketch::configChanged)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject Sketch::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_Sketch.data,
      qt_meta_data_Sketch,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Sketch::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Sketch::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Sketch.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int Sketch::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 194)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 194;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 194)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 194;
    }
    return _id;
}

// SIGNAL 0
void Sketch::configChanged(QString _t1, QVariant _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
