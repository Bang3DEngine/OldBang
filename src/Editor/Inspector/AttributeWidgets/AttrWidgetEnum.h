#ifndef INSPECTORCOMPONENTENUMSLOTWIDGET_H
#define INSPECTORCOMPONENTENUMSLOTWIDGET_H

#include <QComboBox>
#include "Bang/WinUndef.h"

#include "Bang/Array.h"
#include "Bang/InspectorWidget.h"
#include "Bang/AttributeWidget.h"

class ComboBox : public QComboBox
{
public:
    ComboBox(QWidget *parent = nullptr);
private:
    void wheelEvent(QWheelEvent *event) override;
};

template<class EnumClass>
class AttrWidgetEnum  : public AttributeWidget
{
public:
    AttrWidgetEnum(const String &labelText,
                   const Array<EnumClass> &enumValues,
                   const Array<String> &enumNames) :
        AttributeWidget()
    {
        QLayout *layout = new QHBoxLayout();
        m_horizontalLayout.addLayout(layout, 1);

        m_comboBox = new ComboBox(this);
        layout->addWidget(m_comboBox);
        for (const String &enumName : enumNames)
        {
            m_comboBox->addItem( enumName.ToQString() );
        }
        QObject::connect(m_comboBox, SIGNAL(currentIndexChanged(QString)),
                         this, SLOT(OnValueChanged()));

        m_enumValues = enumValues;
        SetHeightSizeHint(35);

        CreateLabel(labelText);
        AfterConstructor();
    }

    virtual void SetValue(EnumClass value)
    {
        m_comboBox->blockSignals(true);
        m_comboBox->setCurrentIndex( m_enumValues.IndexOf(value) );
        m_comboBox->blockSignals(false);
    }

    virtual EnumClass GetValue() const
    {
        return m_enumValues[m_comboBox->currentIndex()];
    }

    virtual void Refresh() override
    {
        AttributeWidget::Refresh();
    }


private:
    ComboBox *m_comboBox = nullptr;
    Array<EnumClass> m_enumValues;
};

#endif // INSPECTORCOMPONENTENUMSLOTWIDGET_H
