#include "stdafx.h"
#include "MergeHashManager.h"

MergeHashManager::MergeHashManager(void *start, void *end, std::deque<std::string>files)
{
    this->start = (char*)start;
    this->end = (char*)end;
    this->merge_files = files;

    this->total_read = 0;
    this->total_write = 0;
}

MergeHashManager::MergeHashManager()
{
}


MergeHashManager::~MergeHashManager()
{
}

DWORD WINAPI mergeExecute(LPVOID data) {
    MergeHash* m = (MergeHash*)data;
    m->execute();

    return 0;
}

void MergeHashManager::execute()
{
    {
        uint32 file_size = ((uint32)std::ceil(((float)this->merge_files.size()) / ((float)MAX_MERGE_THREADS)));
        uint32 offset = (this->end - this->start) / MAX_MERGE_THREADS;
        MergeHash merge[MAX_MERGE_THREADS];
        int prev_size = 0;

        for (int i = 0; i < MAX_MERGE_THREADS; ++i) {
            std::deque<std::string>tmp_files;
            for (int j = 0; j < file_size && this->merge_files.size() > 0; ++j) {
                tmp_files.emplace_back(this->merge_files.front());
                this->merge_files.pop_front();
            }
            (merge + i)->init(this->start + i*offset, this->start + (i + 1)*offset, tmp_files);
            (merge + i)->file_count = i * (prev_size);
            prev_size = tmp_files.size();
        }

        DWORD   dwThreadIdArray[MAX_MERGE_THREADS];
        HANDLE  hThreadArray[MAX_MERGE_THREADS];
        for (int i = 0; i < MAX_MERGE_THREADS; ++i) {
            hThreadArray[i] = CreateThread(
                NULL,                   // default security attributes
                0,                      // use default stack size  
                mergeExecute,         // thread function name
                (merge + i),  // argument to thread function 
                0,                      // use default creation flags 
                &dwThreadIdArray[i]);   // returns the thread identifier 

            if (DEBUG && hThreadArray[i] == NULL)
            {
                //ErrorHandler(TEXT("CreateThread"));
                ExitProcess(3);
            }
        }

        WaitForMultipleObjects(MAX_MERGE_THREADS, hThreadArray, TRUE, INFINITE);

        // Close all thread handles.
        for (int i = 0; i < MAX_MERGE_THREADS; ++i)
        {
            CloseHandle(hThreadArray[i]);
        }

        //At this stage we have just 4 files to merge.
        //Lets give two to each thread.
        //Lets construct two new mergeHash objects to take care of them
        for (int i = 0; i < MAX_MERGE_THREADS; ++i) {
            this->total_read += (merge + i)->total_read;
            this->total_write += (merge + i)->total_write;
            this->merge_files.push_back((merge + i)->merge_files[0]);
        }

    }
    MergeHash merge = MergeHash(this->start, this->end, this->merge_files);
    merge.file_count = 100;
    merge.execute();
    this->merge_files.clear();
    this->merge_files.push_back(merge.merge_files[0]);
    this->total_read += merge.total_read;
    this->total_write += merge.total_write;
}