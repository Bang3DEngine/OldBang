#include "ListInspectorItemWidget.h"
#include "WindowEventManager.h"
#include "Part.h"

//TODO: RENAME THIS CLASS SO IT SHOWS THAT THIS IS RELATED TO A UNIQUE ENTITY PART
ListInspectorPartWidget::ListInspectorPartWidget(Part *relatedPart)
    : QWidget()
{
    this->relatedPart = relatedPart;

    setLayout(new QVBoxLayout());
    layout()->setSpacing(0); layout()->setContentsMargins(0,0,0,0);

    titleLabel = new QLabel(QString::fromStdString(relatedPart->GetName()));
    QFont font = titleLabel->font(); font.setBold(true);
    titleLabel->setFont(font); titleLabel->show();
    layout()->addWidget(titleLabel);

    for(ListInspectorPartSlotInfo *si : relatedPart->GetInfo()->slotInfos)
    {
        WidgetSlot *ws = nullptr;

        ListInspectorPartInfoSlotVecFloat* siv;  //If the infoSlot is of type VecFloat
        if( (siv = dynamic_cast<ListInspectorPartInfoSlotVecFloat*>(si)) != nullptr)
        {
            ws =  new WidgetSlotVectorFloat(siv->value, siv->label, this);
        }

        if(ws != nullptr)
        {
            ws->show();
            layout()->addWidget(ws);
            partSlots.push_back(ws);
            labelsToPartSlots[si->label] = ws;
        }
    }

    this->show();

    updateTimer = new QTimer(this); //Every X seconds, update all the slots values
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(UpdateSlotsValues()));
    updateTimer->start(256);
}

ListInspectorPartWidget::~ListInspectorPartWidget()
{
   delete updateTimer;
}

std::vector<float> ListInspectorPartWidget::GetSlotValueVecFloat(const std::string &slotLabel)
{
    WidgetSlotVectorFloat *w = dynamic_cast<WidgetSlotVectorFloat*>(labelsToPartSlots[slotLabel]);
    std::vector<float> r;
    if(w != nullptr) r = w->GetValue();
    return r;
}

const std::string ListInspectorPartWidget::FloatToString(float f)
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << f;
    return ss.str();
}

ListInspectorPartWidget::WidgetSlotFloat::WidgetSlotFloat(float initialValue,
                                                          const std::string &labelString,
                                                          ListInspectorPartWidget *parent) : WidgetSlot()
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

    spinbox = new QDoubleSpinBox();
    spinbox->setValue(initialValue);
    spinbox->setAlignment(Qt::AlignHCenter);
    spinbox->setMinimum(-999999999.9);
    spinbox->setMaximum(999999999.9);
    spinbox->setAccelerated(true);
    spinbox->setMinimumWidth(50);
    spinbox->setContentsMargins(0,0,0,0);
    spinbox->show();
    connect(spinbox, SIGNAL(valueChanged(double)), parent, SLOT(_NotifyInspectorSlotChanged(double)));

    layout->addWidget(spinbox);

    this->setContentsMargins(0,0,0,0);
    this->show();
}

void ListInspectorPartWidget::WidgetSlotFloat::SetValue(float f)
{
    spinbox->setValue( double(f) );
}

float ListInspectorPartWidget::WidgetSlotFloat::GetValue()
{
    return float(spinbox->value());
}

ListInspectorPartWidget::WidgetSlotVectorFloat::WidgetSlotVectorFloat(std::vector<float> initialValues,
                                                            const std::string &labelString,
                                                            ListInspectorPartWidget *parent) : WidgetSlot()
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
        floatSlots.push_back(s);

        s->setContentsMargins(0,0,0,0); s->show();
        hLayout->addWidget(s);
    }

    this->setContentsMargins(0,0,0,0);
    this->show();
}

void ListInspectorPartWidget::WidgetSlotVectorFloat::SetValue(const std::vector<float> &v)
{
    for(unsigned int i = 0; i < floatSlots.size(); ++i)
    {
        floatSlots[i]->SetValue(v[i]);
    }
}

std::vector<float>  ListInspectorPartWidget::WidgetSlotVectorFloat::GetValue()
{
    std::vector<float> result;
    for(unsigned int i = 0; i < floatSlots.size(); ++i)
    {
        float f = floatSlots[i]->GetValue();
        result.push_back(f);
    }
    return result;
}

void ListInspectorPartWidget::UpdateSlotsValues()
{
    for(ListInspectorPartSlotInfo *si : relatedPart->GetInfo()->slotInfos)
    {
        WidgetSlot *ws = labelsToPartSlots[si->label];

        ListInspectorPartInfoSlotVecFloat* siv;
        if( (siv = dynamic_cast<ListInspectorPartInfoSlotVecFloat*>(si)) != nullptr)
        {
            WidgetSlotVectorFloat *wv = static_cast<WidgetSlotVectorFloat*>(ws);
            wv->SetValue( siv->value );
        }

        if(ws != nullptr)
        {
            ws->show();
            layout()->addWidget(ws);
            partSlots.push_back(ws);
            labelsToPartSlots[si->label] = ws;
        }
    }
}

void ListInspectorPartWidget::_NotifyInspectorSlotChanged(double newValue)
{
    WindowEventManager::NotifyInspectorSlotChanged(relatedPart, this);
}
