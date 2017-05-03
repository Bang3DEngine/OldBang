#include "Bang/FileSystemModel.h"

#include <QPainter>
#include "Bang/WinUndef.h"

#include "Bang/IO.h"
#include "Bang/Debug.h"
#include "Bang/Dialog.h"
#include "Bang/String.h"
#include "Bang/Explorer.h"
#include "Bang/IconManager.h"

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

       bool error = false;
       String errorMsg = "";
       const String calidChars = "/*,;:\\ ";

       Path path( String(filePath(idx)) );
       Path dir = path.GetDirectory();
       String oldFileOrDirNameWithExt = path.GetNameExt();
       String newFileOrDirNameWithExt = value.toString();
       if (path.IsFile())
       {
           String originalExtension = path.GetExtension();

           Path userInput = Path(value.toString());
           String newExtension = userInput.GetExtension();
           newExtension = newExtension.Empty() ? originalExtension : newExtension;
           String newEditedFileName = userInput.GetName();
           String newEditedFileNameExt =
                Path(newEditedFileName).AppendExtension(newExtension).GetAbsolute();
           newFileOrDirNameWithExt = newEditedFileNameExt;

           if (String(value.toString()).Empty())
           {
               error = true;
               errorMsg = "File name is empty.";
           }

           if ( Path(newFileOrDirNameWithExt).Exists())
           {
               error = true;
               errorMsg = "File with that name existed before.";
           }
       }
       if (oldFileOrDirNameWithExt == newFileOrDirNameWithExt) { return false; }

       if (String(value.toString()).IndexOfOneOf(calidChars) >= 0)
       {
           error = true;
           errorMsg = "File/dir name containing invalid characters.";
       }

       if (error)
       {
           Dialog::Error("Error renaming file or dir", errorMsg);
           return false;
       }

       bool ok = IO::Move(dir.Append(oldFileOrDirNameWithExt),
                          dir.Append(newFileOrDirNameWithExt));
       if (ok)
       {
           emit fileRenamed(dir.GetAbsolute().ToQString(),
                            oldFileOrDirNameWithExt.ToQString(),
                            newFileOrDirNameWithExt.ToQString());
       }
       return ok;
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


QVariant FileSystemModel::data(const QModelIndex &idx, int role) const
{
    if (role == Qt::DisplayRole && !m_explorer->IsInListMode())
    {
        String data = QFileSystemModel::data(idx, role).toString();
        String fileName = Path(data).GetName();
        if (data == "..") { return data.ToQString(); }

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
        if (file.GetPath().IsFile())
        {
            File *sFile = File::GetSpecificFile(file);
            if (sFile)
            {
                pm = sFile->GetIcon();
                delete sFile;
            }
        }
        else if (file.GetPath().IsDir())
        {
            // Take the default pixmap and return it scaled
            const QIcon &icon = QFileSystemModel::data(idx, role).value<QIcon>();
            pm = icon.pixmap(m_iconSize, m_iconSize);
        }

        QPixmap pmEmpty(m_iconSize, m_iconSize);
        pmEmpty.fill( Color(1.0f, 1.0f, 1.0f, 0.0f).ToQColor() );
        QPixmap pmScaled = pm.scaled(
                    m_iconSize, m_iconSize,
                    Qt::KeepAspectRatio,
                    Qt::TransformationMode::SmoothTransformation);
        pmScaled = IconManager::CenterPixmapInEmptyPixmap(pmEmpty, pmScaled);
        return pmScaled;
    }
    else if (role == Qt::EditRole)
    {
        if (IO::IsFile(filePath(idx)))
        {
            Path selectedPath = m_explorer->GetSelectedFile().GetPath();
            return QVariant( selectedPath.GetName().ToQString() );
        }
    }
    else if (role == Qt::TextColorRole)
    {
        File file(this, idx);
        if (IO::IsFile(filePath(idx)))
        {
            Color textColor;
            File *sFile = File::GetSpecificFile(file);
            if (sFile && sFile->IsAsset())
            {
                if (sFile->IsAudioClipFile())
                {
                    textColor = Color(1.0f, 1.0f, 0.0f);
                }
                else if (sFile->IsBehaviour())
                {
                    textColor = Color(0.8f, 0.4f, 0.0f);
                }
                else if (sFile->IsMaterialFile())
                {
                    textColor = Color(0.3f, 0.6f, 0.9f);
                }
                else if (sFile->IsMeshFile())
                {
                    textColor = Color(0.8f, 0.5f, 0.8f);
                }
                else
                {
                    textColor = Color(1.0f);
                }
            }
            else
            {
                textColor = Color(0.8f);
            }
            delete sFile;
            return QVariant(textColor.ToQColor());
        }
    }
    else if (role == Qt::FontRole)
    {
        QFont font = QFileSystemModel::data(idx, role).value<QFont>();

        File file(this, idx);
        if ( file.GetPath().IsFile() )
        {
            File *sFile = File::GetSpecificFile(file);
            font.setBold( sFile->IsAsset() );
            delete sFile;
        }

        return QVariant(font);
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
