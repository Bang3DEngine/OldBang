#include "ListInspectorItemWidget.h"
#include "WindowEventManager.h"
#include "Part.h"

//TODO: RENAME THIS CLASS SO IT SHOWS THAT THIS IS RELATED TO A UNIQUE ENTITY PART
ListInspectorItemWidget::ListInspectorItemWidget(Part *relatedPart)
    : QWidget()
{
    this->relatedPart = relatedPart;

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setSpacing(0); mainLayout->setContentsMargins(0,0,0,0);
    setLayout(mainLayout);

    QLabel *titleLabel = new QLabel(QString::fromStdString(relatedPart->GetName()));
    QFont font = titleLabel->font();
    font.setBold(true);
    titleLabel->setFont(font);
    titleLabel->show();
    mainLayout->addWidget(titleLabel);

    for(ListInspectorItemInfoSlot si : relatedPart->inspectorItemInfo.slotInfos)
    {
        if(si.IsVectorTyped())
        {
            std::vector<float> v;
            for(int i = 0; i < si.GetVectorComponentsCount(); ++i) v.push_back(float(i));

            WidgetSlot *w =  new WidgetSlotVector(v, si.label, this);
            w->show();
            mainLayout->addWidget(w);
        }
    }

    this->show();
}

const std::string ListInspectorItemWidget::FloatToString(float f)
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << f;
    return ss.str();
}

ListInspectorItemWidget::WidgetSlotFloat::WidgetSlotFloat(float initialValue,
                                                          const std::string &labelString,
                                                          ListInspectorItemWidget *parent) : WidgetSlot()
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setSpacing(0); layout->setContentsMargins(0,0,0,0);
    this->setLayout(layout);

    if(labelString != "")
    {
        QLabel *textLabel = new QLabel(QString::fromStdString(labelString));
        textLabel->setContentsMargins(0,0,0,0);
        textLabel->show();
        layout->addWidget(textLabel);
    }

    QDoubleSpinBox *sp = new QDoubleSpinBox(); sp->setValue(initialValue);
    connect(sp, SIGNAL(valueChanged(double)), parent, SLOT(_NotifyInspectorSlotChanged(double)));
    sp->setAlignment(Qt::AlignHCenter);
    sp->setMinimum(-999999999.9);
    sp->setMaximum(999999999.9);
    sp->setAccelerated(true);
    sp->setMinimumWidth(50);
    sp->setContentsMargins(0,0,0,0);
    sp->show();
    layout->addWidget(sp);

    this->setContentsMargins(0,0,0,0);
    this->show();
}


ListInspectorItemWidget::WidgetSlotVector::WidgetSlotVector(std::vector<float> initialValues,
                                                            const std::string &labelString,
                                                            ListInspectorItemWidget *parent) : WidgetSlot()
{
    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->setSpacing(0); vLayout->setContentsMargins(0,0,0,0);
    this->setLayout(vLayout);

    QLabel *textLabel = new QLabel(QString::fromStdString(labelString));
    textLabel->setContentsMargins(0,0,0,0);
    vLayout->addWidget(textLabel);

    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->setSpacing(0); hLayout->setContentsMargins(0,0,0,0);
    vLayout->addLayout(hLayout);

    for(unsigned int i = 0; i < initialValues.size(); ++i)
    {
        WidgetSlotFloat *s = new WidgetSlotFloat(initialValues[i], "", parent);
        s->setContentsMargins(0,0,0,0);
        s->show();
        hLayout->addWidget(s);
    }

    this->setContentsMargins(0,0,0,0);
    this->show();
}

void ListInspectorItemWidget::_NotifyInspectorSlotChanged(double newValue)
{
    WindowEventManager::NotifyInspectorSlotChanged(relatedPart);
}
