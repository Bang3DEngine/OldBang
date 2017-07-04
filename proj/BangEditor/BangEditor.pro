include(../BangCommon.pri)
system(rm -f Makefile)
system(cd $$PWD/../.. ; sh ./scripts/preprocessHeaders.sh)

TEMPLATE = lib
INCLUDEPATH += $$PWD/../../include
QT += core gui opengl widgets

CONFIG += staticlib

TARGET = $$BIN_DIR/lib/BangEditor

MOC_DIR = $$PWD/../../bin/Qt_MOCs/

UI_DIR  = $$PWD/../../include/Bang
EDITOR {
    FORMS   += $$PWD/../../res/EngineAssets/EditorWindow.ui
    FORMS   += $$PWD/../../res/EngineAssets/SelectProjectWindow.ui
}

SOURCES += \
    $$PWD/../../src/Editor/General/Screen.cpp \
    $$PWD/../../src/Editor/General/GameObjectClipboard.cpp \
    $$PWD/../../src/Editor/General/Application.cpp \
    $$PWD/../../src/Editor/Windows/IWindow.cpp \
    $$PWD/../../src/Editor/Windows/GameWindow.cpp \
    $$PWD/../../src/Editor/General/Cursor.cpp \
    $$PWD/../../src/Editor/General/Shortcut.cpp \
    $$PWD/../../src/Editor/General/IconManager.cpp

HEADERS += \
    $$PWD/../../src/Editor/General/Application.h \
    $$PWD/../../src/Editor/General/Screen.h

EDITOR {
HEADERS += \
    $$PWD/../../src/Editor/General/GameObjectClipboard.h \
    $$PWD/../../src/Editor/Windows/IWindow.h \
    $$PWD/../../src/Editor/Windows/GameWindow.h \
    $$PWD/../../src/Editor/General/Cursor.h \
    $$PWD/../../src/Editor/General/Shortcut.h \
    $$PWD/../../src/Editor/General/IconManager.h \
    $$PWD/../../src/Editor/Windows/EditorWindow.h \
    $$PWD/../../src/Editor/EditorGameObject/EditorGizmosGameObject.h \
    $$PWD/../../src/Editor/DragDrop/DragDropQWidget.h \
    $$PWD/../../src/Editor/DragDrop/DragDropQListView.h \
    $$PWD/../../src/Editor/Explorer/FileReferencesManager.h \
    $$PWD/../../src/Editor/DragDrop/DragDropQTreeWidget.h \
    $$PWD/../../src/Editor/Inspector/Inspectables/Inspectable.h \
    $$PWD/../../src/Editor/Inspector/Inspectables/GameObjectInspectable.h \
    $$PWD/../../src/Editor/Inspector/Inspectables/IInspectable.h \
    $$PWD/../../src/Editor/Inspector/Inspectables/FileInspectable.h \
    $$PWD/../../src/Editor/Inspector/Inspectables/ComponentInspectable.h \
    $$PWD/../../src/Editor/DragDrop/DragDropQListWidget.h \
    $$PWD/../../src/Editor/DragDrop/DragDropAgent.h \
    $$PWD/../../src/Editor/DragDrop/IDragDropListener.h \
    $$PWD/../../src/Editor/DragDrop/DragDropManager.h \
    $$PWD/../../src/Editor/Explorer/FileSystemModel.h \
    $$PWD/../../src/Editor/EditorGameObject/EditorSelectionGameObject.h \
    $$PWD/../../src/Editor/EditorScene/EditorDebugGameObject.h \
    $$PWD/../../src/Editor/EditorGameObject/EditorTranslateAxis.h \
    $$PWD/../../src/Editor/EditorGameObject/EditorTranslateAxisGroup.h \
    $$PWD/../../src/Editor/Dialogs/DialogBrowseAssetFile.h \
    $$PWD/../../src/Editor/Inspector/AttributeWidgets/AttributeWidget.h \
    $$PWD/../../src/Editor/Inspector/AttributeWidgets/AttrWidgetBool.h \
    $$PWD/../../src/Editor/Inspector/AttributeWidgets/AttrWidgetButton.h \
    $$PWD/../../src/Editor/Inspector/AttributeWidgets/AttrWidgetColor.h \
    $$PWD/../../src/Editor/Inspector/AttributeWidgets/AttrWidgetEnum.h \
    $$PWD/../../src/Editor/Inspector/AttributeWidgets/AttrWidgetFile.h \
    $$PWD/../../src/Editor/Inspector/AttributeWidgets/AttrWidgetFloat.h \
    $$PWD/../../src/Editor/Inspector/AttributeWidgets/AttrWidgetString.h \
    $$PWD/../../src/Editor/Inspector/AttributeWidgets/AttrWidgetVectorFloat.h \
    $$PWD/../../src/Editor/Inspector/AttributeWidgets/IAttrWidgetButtonListener.h \
    $$PWD/../../src/Editor/Inspector/Inspectables/PrefabFileInspectable.h \
    $$PWD/../../src/Editor/Windows/WindowEventManager.h \
    $$PWD/../../src/Editor/Windows/SelectProjectWindow.h \
    $$PWD/../../src/Editor/General/Toolbar.h \
    $$PWD/../../src/Editor/General/EditorState.h \
    $$PWD/../../src/Editor/Dialogs/GameBuildDialog.h \
    $$PWD/../../src/Editor/GameBuilder/GameBuilderJob.h \
    $$PWD/../../src/Editor/Dialogs/Dialog.h \
    $$PWD/../../src/Editor/General/IWindowEventManagerListener.h \
    $$PWD/../../src/Editor/Inspector/Inspector.h \
    $$PWD/../../src/Editor/Hierarchy/Hierarchy.h \
    $$PWD/../../src/Editor/Inspector/ComponentWidget.h \
    $$PWD/../../src/Editor/Console/Console.h \
    $$PWD/../../src/Editor/Explorer/Explorer.h \
    $$PWD/../../src/Editor/General/MenuBar.h \
    $$PWD/../../src/Editor/Explorer/ExplorerDirTree.h \
    $$PWD/../../src/Editor/Inspector/InspectorWidget.h \
    $$PWD/../../src/Editor/EditorCamera/EditorCamera.h \
    $$PWD/../../src/Editor/EditorScene/EditorScene.h \
    $$PWD/../../src/Editor/EditorGameObject/EditorBBox.h \
    $$PWD/../../src/Editor/EditorGameObject/EditorRotateAxisGroup.h \
    $$PWD/../../src/Editor/EditorGameObject/EditorRotateAxis.h \
    $$PWD/../../src/Editor/EditorGameObject/EditorAxisGroup.h \
    $$PWD/../../src/Editor/EditorGameObject/EditorAxis.h \
    $$PWD/../../src/Editor/EditorGameObject/EditorScaleAxisGroup.h \
    $$PWD/../../src/Editor/EditorGameObject/EditorScaleAxis.h \
    $$PWD/../../src/Editor/EditorScene/EditorFloor.h \
    $$PWD/../../src/Editor/Inspector/ComponentClipboard.h \
    $$PWD/../../src/Editor/Hierarchy/HierarchyDragDropManager.h \
    $$PWD/../../src/Editor/Hierarchy/HierarchyContextMenu.h \
    $$PWD/../../src/Editor/Explorer/ExplorerContextMenu.h \
    $$PWD/../../src/Editor/Inspector/ComponentWidgetContextMenu.h \
    $$PWD/../../src/Editor/Hierarchy/HierarchyItem.h \
    $$PWD/../../src/Editor/Inspector/InspectorContextMenu.h \
    $$PWD/../../src/Editor/General/ContextMenu.h \
    $$PWD/../../src/Editor/GameBuilder/GameBuilder.h \
    $$PWD/../../src/Editor/EditorGameObject/EditorAxisPlane.h \
    $$PWD/../../src/Editor/Explorer/ExplorerFileSortProxy.h \
    $$PWD/../../src/Editor/Inspector/AttributeWidgets/AttrWidgetInt.h \
    $$PWD/../../src/Editor/EditorGameObject/EditorRectTransform/EditorRectTransformGizmo.h \
    $$PWD/../../src/Editor/EditorGameObject/EditorRectTransform/EditorRectTransformCornerGizmo.h \
    $$PWD/../../src/Editor/EditorGameObject/EditorRectTransform/EditorRectTransformAnchorGizmo.h \
    $$PWD/../../src/Editor/EditorGameObject/EditorGizmo.h \
    $$PWD/../../src/Editor/DragDrop/DragDropInfo.h \
    $$PWD/../../src/Editor/General/EditorPlayFlow.h
}

EDITOR {
    SOURCES += \
        $$PWD/../../src/Editor/EditorGameObject/EditorGizmosGameObject.cpp \
        $$PWD/../../src/Editor/GameBuilder/GameBuilderJob.cpp \
        $$PWD/../../src/Editor/Dialogs/GameBuildDialog.cpp \
        $$PWD/../../src/Editor/Explorer/FileSystemModel.cpp \
        $$PWD/../../src/Editor/General/EditorState.cpp \
        $$PWD/../../src/Editor/DragDrop/DragDropManager.cpp \
        $$PWD/../../src/Editor/DragDrop/DragDropQWidget.cpp \
        $$PWD/../../src/Editor/Explorer/FileReferencesManager.cpp \
        $$PWD/../../src/Editor/DragDrop/DragDropQListView.cpp \
        $$PWD/../../src/Editor/DragDrop/DragDropQTreeWidget.cpp \
        $$PWD/../../src/Editor/DragDrop/DragDropQListWidget.cpp \
        $$PWD/../../src/Editor/DragDrop/DragDropAgent.cpp \
        $$PWD/../../src/Editor/DragDrop/IDragDropListener.cpp \
        $$PWD/../../src/Editor/EditorGameObject/EditorRotateAxisGroup.cpp \
        $$PWD/../../src/Editor/Windows/EditorWindow.cpp \
        $$PWD/../../src/Editor/Dialogs/DialogBrowseAssetFile.cpp \
        $$PWD/../../src/Editor/Windows/SelectProjectWindow.cpp \
        $$PWD/../../src/Editor/Windows/WindowEventManager.cpp \
        $$PWD/../../src/Editor/EditorGameObject/EditorRotateAxis.cpp \
        $$PWD/../../src/Editor/EditorGameObject/EditorAxisGroup.cpp \
        $$PWD/../../src/Editor/EditorGameObject/EditorAxis.cpp \
        $$PWD/../../src/Editor/EditorGameObject/EditorScaleAxisGroup.cpp \
        $$PWD/../../src/Editor/Inspector/AttributeWidgets/IAttrWidgetButtonListener.cpp \
        $$PWD/../../src/Editor/EditorGameObject/EditorScaleAxis.cpp \
        $$PWD/../../src/Editor/EditorScene/EditorFloor.cpp \
        $$PWD/../../src/Editor/General/IWindowEventManagerListener.cpp \
        $$PWD/../../src/Editor/Inspector/Inspector.cpp \
        $$PWD/../../src/Editor/Hierarchy/Hierarchy.cpp \
        $$PWD/../../src/Editor/Inspector/ComponentWidget.cpp \
        $$PWD/../../src/Editor/Inspector/AttributeWidgets/AttributeWidget.cpp \
        $$PWD/../../src/Editor/Inspector/AttributeWidgets/AttrWidgetFloat.cpp \
        $$PWD/../../src/Editor/Inspector/AttributeWidgets/AttrWidgetVectorFloat.cpp \
        $$PWD/../../src/Editor/Console/Console.cpp \
        $$PWD/../../src/Editor/Inspector/AttributeWidgets/AttrWidgetEnum.cpp \
        $$PWD/../../src/Editor/Explorer/Explorer.cpp \
        $$PWD/../../src/Editor/General/MenuBar.cpp \
        $$PWD/../../src/Editor/Dialogs/Dialog.cpp \
        $$PWD/../../src/Editor/Inspector/AttributeWidgets/AttrWidgetFile.cpp \
        $$PWD/../../src/Editor/Explorer/ExplorerDirTree.cpp \
        $$PWD/../../src/Editor/Inspector/InspectorWidget.cpp \
        $$PWD/../../src/Editor/Inspector/AttributeWidgets/AttrWidgetString.cpp \
        $$PWD/../../src/Editor/EditorCamera/EditorCamera.cpp \
        $$PWD/../../src/Editor/EditorScene/EditorScene.cpp \
        $$PWD/../../src/Editor/EditorGameObject/EditorBBox.cpp \
        $$PWD/../../src/Editor/EditorGameObject/EditorSelectionGameObject.cpp \
        $$PWD/../../src/Editor/EditorScene/EditorDebugGameObject.cpp \
        $$PWD/../../src/Editor/EditorGameObject/EditorTranslateAxisGroup.cpp \
        $$PWD/../../src/Editor/EditorGameObject/EditorTranslateAxis.cpp \
        $$PWD/../../src/Editor/General/Toolbar.cpp \
        $$PWD/../../src/Editor/Inspector/AttributeWidgets/AttrWidgetButton.cpp \
        $$PWD/../../src/Editor/Inspector/AttributeWidgets/AttrWidgetBool.cpp \
        $$PWD/../../src/Editor/Inspector/AttributeWidgets/AttrWidgetColor.cpp \
        $$PWD/../../src/Editor/Inspector/Inspectables/Inspectable.cpp \
        $$PWD/../../src/Editor/Inspector/Inspectables/GameObjectInspectable.cpp \
        $$PWD/../../src/Editor/Inspector/Inspectables/IInspectable.cpp \
        $$PWD/../../src/Editor/Inspector/Inspectables/FileInspectable.cpp \
        $$PWD/../../src/Editor/Inspector/Inspectables/ComponentInspectable.cpp \
        $$PWD/../../src/Editor/Inspector/Inspectables/PrefabFileInspectable.cpp \
        $$PWD/../../src/Editor/Inspector/ComponentClipboard.cpp \
        $$PWD/../../src/Editor/Hierarchy/HierarchyDragDropManager.cpp \
        $$PWD/../../src/Editor/Hierarchy/HierarchyContextMenu.cpp \
        $$PWD/../../src/Editor/Explorer/ExplorerContextMenu.cpp \
        $$PWD/../../src/Editor/Inspector/ComponentWidgetContextMenu.cpp \
        $$PWD/../../src/Editor/Inspector/InspectorContextMenu.cpp \
        $$PWD/../../src/Editor/GameBuilder/GameBuilder.cpp \
        $$PWD/../../src/Editor/General/ContextMenu.cpp \
        $$PWD/../../src/Editor/Hierarchy/HierarchyItem.cpp \
        $$PWD/../../src/Editor/Explorer/ExplorerFileSortProxy.cpp \
        $$PWD/../../src/Editor/Inspector/AttributeWidgets/AttrWidgetInt.cpp \
        $$PWD/../../src/Editor/EditorGameObject/EditorRectTransform/EditorRectTransformGizmo.cpp \
        $$PWD/../../src/Editor/EditorGameObject/EditorRectTransform/EditorRectTransformCornerGizmo.cpp \
        $$PWD/../../src/Editor/EditorGameObject/EditorRectTransform/EditorRectTransformAnchorGizmo.cpp \
        $$PWD/../../src/Editor/EditorGameObject/EditorGizmo.cpp \
        $$PWD/../../src/Editor/EditorGameObject/EditorAxisPlane.cpp \
        $$PWD/../../src/Editor/General/EditorPlayFlow.cpp
}
######################################
