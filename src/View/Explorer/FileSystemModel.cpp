#include "FileSystemModel.h"

#include "Debug.h"
#include "String.h"
#include "Explorer.h"

FileSystemModel::FileSystemModel(Explorer *explorer) :
    QFileSystemModel()
{
    setFilter(QDir::AllEntries | QDir::NoDot);
    setNameFilterDisables(false);
    setReadOnly(false);

    m_explorer = explorer;
}

void FileSystemModel::SetIconSize(int iconSize)
{
    m_iconSize = iconSize;
}

Qt::DropActions FileSystemModel::supportedDropActions() const
{
    return Qt::MoveAction;
}

bool FileSystemModel::setData(const QModelIndex &idx,
                              const QVariant &value,
                              int role)
{
   if (role == Qt::EditRole)
   {
       String path = filePath(idx);
       if (Persistence::IsFile(path))
       {
           String originalExtension = Persistence::GetFileExtensionComplete(path);

           String userInput = value.toString();
           String newExtension = Persistence::GetFileExtensionComplete(userInput);
           newExtension = newExtension.Empty() ? originalExtension : newExtension;
           String newEditedFileName = Persistence::GetFileName(userInput);
           String newEditedFileNameExt = newEditedFileName + "." + newExtension;
           String dir = Persistence::GetDir(path);
           String newPath = dir + "/" + newEditedFileNameExt;
           if (path == newPath) { return false; }

           // File name checks
           bool error = false;
           String errorMsg = "";
           String invalidChars = "/*,;:\\ ";
           if (String(value.toString()).IndexOfOneOf(invalidChars) >= 0)
           {
               error = true;
               errorMsg = "File name containing invalid characters.";
           }

           if (String(value.toString()).Empty())
           {
               error = true;
               errorMsg = "File name is empty.";
           }

           if (Persistence::Exists(newPath))
           {
               error = true;
               errorMsg = "File with that name existed before.";
           }

           if (error)
           {
               Dialog::Error("Error renaming file", errorMsg);
               return false;
           }

           bool ok = Persistence::Move(path, newPath);
           if (ok)
           {
               String originalFileNameExt =
                       Persistence::GetFileNameWithExtension(path);
               emit fileRenamed(dir.ToQString(),
                                originalFileNameExt.ToQString(),
                                newEditedFileNameExt.ToQString());
           }
           return ok;
       }
   }
   return QFileSystemModel::setData(idx, value, role);
}

QSize FileSystemModel::GetCharSize() const
{
    QFontMetrics fm = m_explorer->fontMetrics();
    return QSize(fm.averageCharWidth(),
                 fm.height());
}

int FileSystemModel::GetWordWrappingWidth() const
{
    return m_iconSize * 2.0f;
}

int FileSystemModel::GetWordWrappingCharsPerLine() const
{
    int containerWidth = GetWordWrappingWidth();
    const int charWidth = GetCharSize().width();
    return std::max(1, containerWidth / charWidth);
}


QVariant FileSystemModel::data(const QModelIndex &idx,
                               int role) const
{
    if (role == Qt::DisplayRole && !m_explorer->IsInListMode())
    {
        String data = QFileSystemModel::data(idx, role).toString();
        String fileName = Persistence::GetFileName(data);

        const int justInCase = 4; // To avoid char clamping for some cases
        const int charsPerLine = GetWordWrappingCharsPerLine() - justInCase;
        for (int i = 1; i < fileName.Length() / charsPerLine + 1; ++i)
        {
            int breakInsertPos = charsPerLine * i + (i-1);
            if (fileName.Length() >= breakInsertPos)
            {
                fileName.Insert(breakInsertPos, "\n");
            }
        }
        return fileName.ToQString();
    }
    else if (role == Qt::DecorationRole)
    {
        File file(this, idx);

        QPixmap pm;
        String absPath = file.GetAbsolutePath();
        if (Persistence::IsFile(absPath))
        {
            File *sFile = File::GetSpecificFile(file);
            if (sFile)
            {
                pm = sFile->GetIcon();
                delete sFile;
            }
        }
        else if (Persistence::IsDir(absPath))
        {
            // Take the default pixmap and return it scaled
            const QIcon &icon = QFileSystemModel::data(idx, role).value<QIcon>();
            pm = icon.pixmap(m_iconSize, m_iconSize);
        }

        QPixmap pmScaled = pm.scaled(
                    m_iconSize, m_iconSize,
                    Qt::IgnoreAspectRatio,
                    Qt::TransformationMode::SmoothTransformation);

        if (Persistence::IsFile(absPath))
        {
            File *sFile = File::GetSpecificFile(file);
            pmScaled = File::AddIconAssetTypeDistinctor(pmScaled, sFile->IsAsset());
            delete sFile;
        }

        return pmScaled;
    }
    else if (role == Qt::EditRole)
    {
        if (Persistence::IsFile(filePath(idx)))
        {
            String fileName = m_explorer->GetSelectedFile().GetName();
            return QVariant( fileName.ToQString() );
        }
    }
    else if (role == Qt::SizeHintRole)
    {
        if (!m_explorer->IsInListMode())
        {
            const int c_numLines = 2;
            const int width      = GetWordWrappingWidth();
            const int height     = GetCharSize().height() * c_numLines +
                                   (m_iconSize * 1.3f);
            return QSize(width, height);
        }
        else
        {
            QSize defaultSize = QFileSystemModel::data(idx, role).toSize();
            return QSize(defaultSize.width(), m_iconSize);
        }
    }
    else if (role == Qt::TextAlignmentRole)
    {
        return m_explorer->IsInListMode() ?
                    int(Qt::AlignVCenter | Qt::AlignLeft) :
                    int(Qt::AlignTop     | Qt::AlignHCenter);
    }

    return QFileSystemModel::data(idx, role);
}
