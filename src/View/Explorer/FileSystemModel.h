#ifndef FILESYSTEMMODEL_H
#define FILESYSTEMMODEL_H

#include <QFileSystemModel>

class Explorer;
class FileSystemModel : public QFileSystemModel
{
public:
    FileSystemModel(Explorer *explorer);

    void SetIconSize(int iconSize);

protected:
    Explorer *m_explorer = nullptr;

    Qt::DropActions supportedDropActions() const override;

    QVariant data(const QModelIndex& idx, int role) const override;
    virtual bool setData(const QModelIndex &idx,
                         const QVariant &value,
                         int role = Qt::EditRole) override;

    QSize GetCharSize() const;
    int GetWordWrappingWidth() const;
    int GetWordWrappingCharsPerLine() const;

private:
    int m_iconSize = 50;
};

#endif // FILESYSTEMMODEL_H
