/** \file WriteThread.h
\brief Thread changed to open/close and write the destination file
\author alpha_one_x86
\licence GPL3, see the file COPYING */

#ifndef WRITETHREAD_H
#define WRITETHREAD_H

#include <QThread>
#include <QByteArray>
#include <QString>
#include <QCryptographicHash>

#include "Environment.h"
#include "StructEnumDefinition_CopyEngine.h"
#include "AvancedQFile.h"

/// \brief Thread changed to open/close and write the destination file
class WriteThread : public QObject
{
    Q_OBJECT
public:
    explicit WriteThread();
    ~WriteThread();
protected:
    void run();
public:
    /// \brief open the destination to open it
    void open(const QFileInfo &file,const uint64_t &startSize,const bool &buffer,const int &numberOfBlock,const bool &sequential);
    /// \brief to return the error string
    std::string errorString() const;
    /// \brief to stop all
    void stop();
    /// \brief to write data
    bool write(const QByteArray &data);
    #ifdef ULTRACOPIER_PLUGIN_DEBUG
    /// \brief to set the id
    void setId(int id);
    /// \brief get the write stat
    enum WriteStat
    {
        Idle=0,
        InodeOperation=1,
        Write=2,
        Close=3,
        Read=5,
        Checksum=6
    };
    WriteStat stat;
    #endif
    /// \brief do the fake open
    void fakeOpen();
    /// \brief do the fake writeIsStarted
    void fakeWriteIsStarted();
    /// \brief do the fake writeIsStopped
    void fakeWriteIsStopped();
    /// do the checksum
    void startCheckSum();
    /// \brief set block size in KB
    bool setBlockSize(const int blockSize);
    /// \brief get the last good position
    int64_t getLastGoodPosition() const;
    /// \brief buffer is empty
    bool bufferIsEmpty();
    void reemitStartOpen();
public slots:
    /// \brief start the operation
    void postOperation();
    /// \brief flush buffer
    void flushBuffer();
    /// \brief set the end is detected
    void endIsDetected();
    /// \brief reopen the file
    void reopen();
    /// \brief flush and seek to zero
    void flushAndSeekToZero();
    /// do the checksum
    void checkSum();
    void setDeletePartiallyTransferredFiles(const bool &deletePartiallyTransferredFiles);
    /// \brief executed at regular interval to do a speed throling
    void timeOfTheBlockCopyFinished();

    void resumeNotStarted();
signals:
    void error() const;
    void opened() const;
    void reopened() const;
    void writeIsStarted() const;
    void writeIsStopped() const;
    void flushedAndSeekedToZero() const;
    void closed() const;
    void checksumFinish(const QByteArray&) const;
    //internal signals
    void internalStartOpen() const;
    void internalStartChecksum() const;
    void internalStartReopen() const;
    void internalStartWrite() const;
    void internalStartClose() const;
    void internalStartEndOfFile() const;
    void internalStartFlushAndSeekToZero() const;
    /// \brief To debug source
    void debugInformation(const Ultracopier::DebugLevel &level,const std::string &fonction,const std::string &text,const std::string &file,const int &ligne) const;
private:
    std::string             errorString_internal;
    AvancedQFile		file;
    volatile bool		stopIt;
    volatile bool       postOperationRequested;
    volatile int		blockSize;//only used in checksum
    int                 numberOfBlock;
    static QMultiHash<QString,WriteThread *> writeFileList;
    volatile bool       writeFullBlocked;
    uint64_t             lastGoodPosition;
    QByteArray          blockArray;		///< temp data for block writing, the data
    int64_t              bytesWriten;		///< temp data for block writing, the bytes writen
    int                 id;
    volatile bool       endDetected;
    uint64_t             startSize;
    bool                fakeMode;
    bool                buffer;
    bool                needRemoveTheFile;
    volatile bool       sequential;
    bool                deletePartiallyTransferredFiles;
private slots:
    bool internalOpen();
    void internalWrite();
    void internalCloseSlot();
    void internalClose(bool emitSignal=true);
    void internalReopen();
    void internalEndOfFile();
    void internalFlushAndSeekToZero();
};

#endif // WRITETHREAD_H
