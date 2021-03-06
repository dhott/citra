// Copyright 2014 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include <QAbstractListModel>

#include "graphics_breakpoint_observer.h"

#include "nihstro/parser_shbin.h"

#include "video_core/shader/shader.h"

class QLabel;
class QSpinBox;

class GraphicsVertexShaderWidget;

class GraphicsVertexShaderModel : public QAbstractItemModel {
    Q_OBJECT

public:
    GraphicsVertexShaderModel(GraphicsVertexShaderWidget* parent);

    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& child) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
    GraphicsVertexShaderWidget* par;

    friend class GraphicsVertexShaderWidget;
};

class GraphicsVertexShaderWidget : public BreakPointObserverDock {
    Q_OBJECT

    using Event = Pica::DebugContext::Event;

public:
    GraphicsVertexShaderWidget(std::shared_ptr<Pica::DebugContext> debug_context,
                               QWidget* parent = nullptr);

private slots:
    void OnBreakPointHit(Pica::DebugContext::Event event, void* data) override;
    void OnResumed() override;

    void OnInputAttributeChanged(int index);

    void OnCycleIndexChanged(int index);

    void DumpShader();

    /**
     * Reload widget based on the current PICA200 state
     * @param replace_vertex_data If true, invalidate all current vertex data
     * @param vertex_data New vertex data to use, as passed to OnBreakPointHit. May be nullptr to specify that no valid vertex data can be retrieved currently. Only used if replace_vertex_data is true.
     */
    void Reload(bool replace_vertex_data = false, void* vertex_data = nullptr);


signals:
    // Call this to change the current command selection in the disassembly view
    void SelectCommand(const QModelIndex&, QItemSelectionModel::SelectionFlags);

private:
    QLabel* instruction_description;
    QTreeView* binary_list;
    GraphicsVertexShaderModel* model;

    /// TODO: Move these into a single struct
    std::array<QLineEdit*, 4*16> input_data;  // A text box for each of the 4 components of up to 16 vertex attributes
    std::array<QWidget*, 16> input_data_container; // QWidget containing the QLayout containing each vertex attribute
    std::array<QLabel*, 16> input_data_mapping; // A QLabel denoting the shader input attribute which the vertex attribute maps to

    // Text to be shown when input vertex data is not retrievable
    QLabel* breakpoint_warning;

    QSpinBox* cycle_index;

    nihstro::ShaderInfo info;
    Pica::Shader::DebugData<true> debug_data;
    Pica::Shader::InputVertex input_vertex;

    friend class GraphicsVertexShaderModel;
};
