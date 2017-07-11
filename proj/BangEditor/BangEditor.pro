include(../BangCommon.pri)
system(rm -f Makefile)
system(cd $$BANG_ROOT ; sh ./scripts/preprocessHeaders.sh)

TEMPLATE = lib
CONFIG += staticlib
QT += core gui opengl widgets

UI_DIR  = $$BANG_ROOT/include/Bang/UI_Qt

INCLUDEPATH += \
    $$BANG_ROOT/include/Bang/BangGraphics \
    $$BANG_ROOT/include/Bang/BangEngine \
    $$BANG_ROOT/include/Bang/BangEditor \
    $$UI_DIR

TARGET = $$BIN_DIR/lib/BangEditor

MOC_DIR = $$BANG_ROOT/bin/Qt_MOCs/
FORMS   += $$BANG_ROOT/res/EngineAssets/EditorWindow.ui
FORMS   += $$BANG_ROOT/res/EngineAssets/SelectProjectWindow.ui

SOURCES += \
    $$BANG_ROOT/src/Editor/General/GameObjectClipboard.cpp \
    $$BANG_ROOT/src/Editor/General/IconManager.cpp \
    $$BANG_ROOT/src/Engine/IO/Files/AssetFileCreator.cpp \
    $$BANG_ROOT/src/Editor/General/EditorApplication.cpp \
    $$BANG_ROOT/src/Engine/IO/Behaviours/BehaviourRefresherTimer.cpp \
    $$BANG_ROOT/src/Editor/EditorGameObject/EditorGizmosGameObject.cpp \
    $$BANG_ROOT/src/Editor/GameBuilder/GameBuilderJob.cpp \
    $$BANG_ROOT/src/Editor/Inspector/AudioSourceInspectorWidget.cpp \
    $$BANG_ROOT/src/Editor/Dialogs/GameBuildDialog.cpp \
    $$BANG_ROOT/src/Editor/Explorer/FileSystemModel.cpp \
    $$BANG_ROOT/src/Editor/General/EditorState.cpp \
    $$BANG_ROOT/src/Editor/DragDrop/DragDropManager.cpp \
    $$BANG_ROOT/src/Editor/DragDrop/DragDropQWidget.cpp \
    $$BANG_ROOT/src/Editor/Explorer/FileReferencesManager.cpp \
    $$BANG_ROOT/src/Editor/DragDrop/DragDropQListView.cpp \
    $$BANG_ROOT/src/Editor/DragDrop/DragDropQTreeWidget.cpp \
    $$BANG_ROOT/src/Editor/DragDrop/DragDropQListWidget.cpp \
    $$BANG_ROOT/src/Editor/DragDrop/DragDropAgent.cpp \
    $$BANG_ROOT/src/Editor/DragDrop/IDragDropListener.cpp \
    $$BANG_ROOT/src/Engine/IO/Project/QtProjectManager.cpp \
    $$BANG_ROOT/src/Editor/EditorGameObject/EditorRotateAxisGroup.cpp \
    $$BANG_ROOT/src/Editor/Windows/EditorWindow.cpp \
    $$BANG_ROOT/src/Editor/Dialogs/DialogBrowseAssetFile.cpp \
    $$BANG_ROOT/src/Editor/Windows/SelectProjectWindow.cpp \
    $$BANG_ROOT/src/Editor/Windows/WindowEventManager.cpp \
    $$BANG_ROOT/src/Editor/EditorGameObject/EditorRotateAxis.cpp \
    $$BANG_ROOT/src/Editor/EditorGameObject/EditorAxisGroup.cpp \
    $$BANG_ROOT/src/Editor/EditorGameObject/EditorAxis.cpp \
    $$BANG_ROOT/src/Editor/EditorGameObject/EditorScaleAxisGroup.cpp \
    $$BANG_ROOT/src/Editor/Inspector/AttributeWidgets/IAttrWidgetButtonListener.cpp \
    $$BANG_ROOT/src/Editor/EditorGameObject/EditorScaleAxis.cpp \
    $$BANG_ROOT/src/Editor/EditorScene/EditorFloor.cpp \
    $$BANG_ROOT/src/Editor/General/IWindowEventManagerListener.cpp \
    $$BANG_ROOT/src/Editor/Inspector/Inspector.cpp \
    $$BANG_ROOT/src/Editor/Hierarchy/Hierarchy.cpp \
    $$BANG_ROOT/src/Editor/General/Shortcut.cpp \
    $$BANG_ROOT/src/Editor/Graphics/EditorScreen.cpp \
    $$BANG_ROOT/src/Editor/Inspector/ComponentWidget.cpp \
    $$BANG_ROOT/src/Editor/Inspector/AttributeWidgets/AttributeWidget.cpp \
    $$BANG_ROOT/src/Editor/Inspector/AttributeWidgets/AttrWidgetFloat.cpp \
    $$BANG_ROOT/src/Editor/Inspector/AttributeWidgets/AttrWidgetVectorFloat.cpp \
    $$BANG_ROOT/src/Editor/Console/Console.cpp \
    $$BANG_ROOT/src/Engine/IO/Files/FileTracker.cpp \
    $$BANG_ROOT/src/Editor/General/EditorProjectManager.cpp \
    $$BANG_ROOT/src/Editor/Inspector/AttributeWidgets/AttrWidgetEnum.cpp \
    $$BANG_ROOT/src/Editor/Explorer/Explorer.cpp \
    $$BANG_ROOT/src/Editor/General/MenuBar.cpp \
    $$BANG_ROOT/src/Editor/Dialogs/Dialog.cpp \
    $$BANG_ROOT/src/Editor/Inspector/AttributeWidgets/AttrWidgetFile.cpp \
    $$BANG_ROOT/src/Editor/Explorer/ExplorerDirTree.cpp \
    $$BANG_ROOT/src/Editor/Inspector/InspectorWidget.cpp \
    $$BANG_ROOT/src/Editor/Inspector/InspectorWidgetFactory.cpp \
    $$BANG_ROOT/src/Editor/Inspector/AttributeWidgets/AttrWidgetString.cpp \
    $$BANG_ROOT/src/Editor/EditorCamera/EditorCamera.cpp \
    $$BANG_ROOT/src/Editor/EditorScene/EditorScene.cpp \
    $$BANG_ROOT/src/Editor/EditorGameObject/EditorBBox.cpp \
    $$BANG_ROOT/src/Editor/EditorGameObject/EditorSelectionGameObject.cpp \
    $$BANG_ROOT/src/Editor/EditorScene/EditorDebugGameObject.cpp \
    $$BANG_ROOT/src/Editor/EditorGameObject/EditorTranslateAxisGroup.cpp \
    $$BANG_ROOT/src/Editor/EditorGameObject/EditorTranslateAxis.cpp \
    $$BANG_ROOT/src/Editor/General/Toolbar.cpp \
    $$BANG_ROOT/src/Editor/Graphics/GPPass_Selection.cpp \
    $$BANG_ROOT/src/Editor/Graphics/SelectionFramebuffer.cpp \
    $$BANG_ROOT/src/Editor/Inspector/AttributeWidgets/AttrWidgetButton.cpp \
    $$BANG_ROOT/src/Editor/Inspector/AttributeWidgets/AttrWidgetBool.cpp \
    $$BANG_ROOT/src/Editor/Inspector/AttributeWidgets/AttrWidgetColor.cpp \
    $$BANG_ROOT/src/Editor/Inspector/Inspectables/Inspectable.cpp \
    $$BANG_ROOT/src/Editor/Inspector/Inspectables/GameObjectInspectable.cpp \
    $$BANG_ROOT/src/Editor/Inspector/Inspectables/IInspectable.cpp \
    $$BANG_ROOT/src/Editor/Inspector/Inspectables/FileInspectable.cpp \
    $$BANG_ROOT/src/Editor/Inspector/Inspectables/ComponentInspectable.cpp \
    $$BANG_ROOT/src/Editor/Inspector/Inspectables/PrefabFileInspectable.cpp \
    $$BANG_ROOT/src/Editor/Inspector/ComponentClipboard.cpp \
    $$BANG_ROOT/src/Editor/Hierarchy/HierarchyDragDropManager.cpp \
    $$BANG_ROOT/src/Editor/Hierarchy/HierarchyContextMenu.cpp \
    $$BANG_ROOT/src/Editor/Explorer/ExplorerContextMenu.cpp \
    $$BANG_ROOT/src/Editor/Inspector/ComponentWidgetContextMenu.cpp \
    $$BANG_ROOT/src/Editor/Inspector/InspectorContextMenu.cpp \
    $$BANG_ROOT/src/Editor/GameBuilder/GameBuilder.cpp \
    $$BANG_ROOT/src/Editor/General/ContextMenu.cpp \
    $$BANG_ROOT/src/Editor/Hierarchy/HierarchyItem.cpp \
    $$BANG_ROOT/src/Editor/Explorer/ExplorerFileSortProxy.cpp \
    $$BANG_ROOT/src/Editor/Inspector/AttributeWidgets/AttrWidgetInt.cpp \
    $$BANG_ROOT/src/Editor/EditorGameObject/EditorRectTransform/EditorRectTransformGizmo.cpp \
    $$BANG_ROOT/src/Editor/EditorGameObject/EditorRectTransform/EditorRectTransformCornerGizmo.cpp \
    $$BANG_ROOT/src/Editor/EditorGameObject/EditorRectTransform/EditorRectTransformAnchorGizmo.cpp \
    $$BANG_ROOT/src/Editor/EditorGameObject/EditorGizmo.cpp \
    $$BANG_ROOT/src/Editor/EditorGameObject/EditorAxisPlane.cpp \
    $$BANG_ROOT/src/Editor/General/EditorPlayFlow.cpp

HEADERS += \
    $$BANG_ROOT/src/Editor/General/IconManager.h \
    $$BANG_ROOT/src/Engine/IO/Files/AssetFileCreator.h \
    $$BANG_ROOT/src/Engine/IO/Behaviours/BehaviourRefresherTimer.h \
    $$BANG_ROOT/src/Editor/General/GameObjectClipboard.h \
    $$BANG_ROOT/src/Engine/IO/Files/FileTracker.h \
    $$BANG_ROOT/src/Editor/General/Shortcut.h \
    $$BANG_ROOT/src/Editor/Windows/EditorWindow.h \
    $$BANG_ROOT/src/Editor/EditorGameObject/EditorGizmosGameObject.h \
    $$BANG_ROOT/src/Editor/DragDrop/DragDropQWidget.h \
    $$BANG_ROOT/src/Editor/DragDrop/DragDropQListView.h \
    $$BANG_ROOT/src/Editor/Explorer/FileReferencesManager.h \
    $$BANG_ROOT/src/Editor/Graphics/EditorScreen.h \
    $$BANG_ROOT/src/Editor/DragDrop/DragDropQTreeWidget.h \
    $$BANG_ROOT/src/Editor/Inspector/Inspectables/Inspectable.h \
    $$BANG_ROOT/src/Editor/Inspector/Inspectables/GameObjectInspectable.h \
    $$BANG_ROOT/src/Editor/Inspector/Inspectables/IInspectable.h \
    $$BANG_ROOT/src/Editor/Inspector/Inspectables/FileInspectable.h \
    $$BANG_ROOT/src/Editor/Inspector/Inspectables/ComponentInspectable.h \
    $$BANG_ROOT/src/Editor/DragDrop/DragDropQListWidget.h \
    $$BANG_ROOT/src/Editor/DragDrop/DragDropAgent.h \
    $$BANG_ROOT/src/Editor/General/EditorProjectManager.h \
    $$BANG_ROOT/src/Engine/IO/Project/QtProjectManager.h \
    $$BANG_ROOT/src/Editor/DragDrop/IDragDropListener.h \
    $$BANG_ROOT/src/Editor/DragDrop/DragDropManager.h \
    $$BANG_ROOT/src/Editor/Explorer/FileSystemModel.h \
    $$BANG_ROOT/src/Editor/Graphics/GPPass_Selection.h \
    $$BANG_ROOT/src/Editor/Graphics/SelectionFramebuffer.h \
    $$BANG_ROOT/src/Editor/EditorGameObject/EditorSelectionGameObject.h \
    $$BANG_ROOT/src/Editor/EditorScene/EditorDebugGameObject.h \
    $$BANG_ROOT/src/Editor/EditorGameObject/EditorTranslateAxis.h \
    $$BANG_ROOT/src/Editor/EditorGameObject/EditorTranslateAxisGroup.h \
    $$BANG_ROOT/src/Editor/Dialogs/DialogBrowseAssetFile.h \
    $$BANG_ROOT/src/Editor/Inspector/AttributeWidgets/AttributeWidget.h \
    $$BANG_ROOT/src/Editor/Inspector/AttributeWidgets/AttrWidgetBool.h \
    $$BANG_ROOT/src/Editor/Inspector/AttributeWidgets/AttrWidgetButton.h \
    $$BANG_ROOT/src/Editor/Inspector/AttributeWidgets/AttrWidgetColor.h \
    $$BANG_ROOT/src/Editor/Inspector/AttributeWidgets/AttrWidgetEnum.h \
    $$BANG_ROOT/src/Editor/Inspector/AttributeWidgets/AttrWidgetFile.h \
    $$BANG_ROOT/src/Editor/Inspector/AttributeWidgets/AttrWidgetFloat.h \
    $$BANG_ROOT/src/Editor/Inspector/AttributeWidgets/AttrWidgetString.h \
    $$BANG_ROOT/src/Editor/Inspector/AttributeWidgets/AttrWidgetVectorFloat.h \
    $$BANG_ROOT/src/Editor/Inspector/AttributeWidgets/IAttrWidgetButtonListener.h \
    $$BANG_ROOT/src/Editor/Inspector/Inspectables/PrefabFileInspectable.h \
    $$BANG_ROOT/src/Editor/Windows/WindowEventManager.h \
    $$BANG_ROOT/src/Editor/Windows/SelectProjectWindow.h \
    $$BANG_ROOT/src/Editor/General/Toolbar.h \
    $$BANG_ROOT/src/Editor/General/EditorState.h \
    $$BANG_ROOT/src/Editor/Dialogs/GameBuildDialog.h \
    $$BANG_ROOT/src/Editor/GameBuilder/GameBuilderJob.h \
    $$BANG_ROOT/src/Editor/Dialogs/Dialog.h \
    $$BANG_ROOT/src/Editor/General/IWindowEventManagerListener.h \
    $$BANG_ROOT/src/Editor/Inspector/Inspector.h \
    $$BANG_ROOT/src/Editor/Hierarchy/Hierarchy.h \
    $$BANG_ROOT/src/Editor/Inspector/ComponentWidget.h \
    $$BANG_ROOT/src/Editor/Console/Console.h \
    $$BANG_ROOT/src/Editor/Explorer/Explorer.h \
    $$BANG_ROOT/src/Editor/General/MenuBar.h \
    $$BANG_ROOT/src/Editor/Explorer/ExplorerDirTree.h \
    $$BANG_ROOT/src/Editor/Inspector/InspectorWidget.h \
    $$BANG_ROOT/src/Editor/Inspector/InspectorWidgetFactory.h \
    $$BANG_ROOT/src/Editor/EditorCamera/EditorCamera.h \
    $$BANG_ROOT/src/Editor/EditorScene/EditorScene.h \
    $$BANG_ROOT/src/Editor/EditorGameObject/EditorBBox.h \
    $$BANG_ROOT/src/Editor/EditorGameObject/EditorRotateAxisGroup.h \
    $$BANG_ROOT/src/Editor/EditorGameObject/EditorRotateAxis.h \
    $$BANG_ROOT/src/Editor/EditorGameObject/EditorAxisGroup.h \
    $$BANG_ROOT/src/Editor/EditorGameObject/EditorAxis.h \
    $$BANG_ROOT/src/Editor/EditorGameObject/EditorScaleAxisGroup.h \
    $$BANG_ROOT/src/Editor/EditorGameObject/EditorScaleAxis.h \
    $$BANG_ROOT/src/Editor/Inspector/AudioSourceInspectorWidget.h \
    $$BANG_ROOT/src/Editor/EditorScene/EditorFloor.h \
    $$BANG_ROOT/src/Editor/Inspector/ComponentClipboard.h \
    $$BANG_ROOT/src/Editor/Hierarchy/HierarchyDragDropManager.h \
    $$BANG_ROOT/src/Editor/Hierarchy/HierarchyContextMenu.h \
    $$BANG_ROOT/src/Editor/Explorer/ExplorerContextMenu.h \
    $$BANG_ROOT/src/Editor/Inspector/ComponentWidgetContextMenu.h \
    $$BANG_ROOT/src/Editor/Hierarchy/HierarchyItem.h \
    $$BANG_ROOT/src/Editor/Inspector/InspectorContextMenu.h \
    $$BANG_ROOT/src/Editor/General/ContextMenu.h \
    $$BANG_ROOT/src/Editor/GameBuilder/GameBuilder.h \
    $$BANG_ROOT/src/Editor/EditorGameObject/EditorAxisPlane.h \
    $$BANG_ROOT/src/Editor/Explorer/ExplorerFileSortProxy.h \
    $$BANG_ROOT/src/Editor/Inspector/AttributeWidgets/AttrWidgetInt.h \
    $$BANG_ROOT/src/Editor/EditorGameObject/EditorRectTransform/EditorRectTransformGizmo.h \
    $$BANG_ROOT/src/Editor/EditorGameObject/EditorRectTransform/EditorRectTransformCornerGizmo.h \
    $$BANG_ROOT/src/Editor/EditorGameObject/EditorRectTransform/EditorRectTransformAnchorGizmo.h \
    $$BANG_ROOT/src/Editor/EditorGameObject/EditorGizmo.h \
    $$BANG_ROOT/src/Editor/DragDrop/DragDropInfo.h \
    $$BANG_ROOT/src/Editor/General/EditorPlayFlow.h

######################################
