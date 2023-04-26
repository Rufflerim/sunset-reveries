//
// Created by gaetz on 25/04/2023.
//

#include "Jobs.hpp"

#include <algorithm>
#include <atomic>
#include <thread>
#include <condition_variable>



namespace jobs
{

    /**
    *  Number of worker threads, it will be initialized in the Initialize() function
    */
    u32 numThreads = 0;

    /**
     * A thread safe queue to put pending jobs onto the end (with a capacity of 256 jobs).
     * A worker thread can grab a job from the beginning.
     */
    ThreadSafeRingBuffer<std::function<void()>, 256> jobPool;

    /**
     * Used in conjunction with the wakeMutex.
     * Worker threads just sleep when there is no job, and the main thread can wake them up.
     */
    std::condition_variable wakeCondition;

    /**
     * Used in conjunction with the wakeCondition.
     */
    std::mutex wakeMutex;

    /** Tracks the state of execution of the main thread */
    u64 currentLabel = 0;

    /** Track the state of execution across background worker threads */
    std::atomic<uint64_t> finishedLabel;

    void Initialize() {
        // Initialize the worker execution state to 0:
        finishedLabel.store(0);

        // Retrieve the number of hardware threads in this system:
        auto numCores = std::thread::hardware_concurrency();

        // Calculate the actual number of worker threads we want:
        numThreads = std::max(1u, numCores);

        // Create all our worker threads while immediately starting them:
        for (uint32_t threadID = 0; threadID < numThreads; ++threadID) {
            std::thread worker([] {
                std::function<void()> job; // the current job for the thread, it's empty at start.

                // This is the infinite loop that a worker thread will do
                while (true) {
                    // Try to grab a job from the jobPool queue
                    if (jobPool.pop_front(job)) {
                        // It found a job, execute it:
                        job();
                        // Update worker label state
                        finishedLabel.fetch_add(1);
                    } else {
                        // No job, put thread to sleep
                        std::unique_lock<std::mutex> lock(wakeMutex);
                        wakeCondition.wait(lock);
                    }
                }
            });

            worker.detach(); // forget about this thread, let it do its job in the infinite loop that we created above
        }
    }

    /**
     * This little helper function will not let the system to be deadlocked
     * while the main thread is waiting for something
     * */
    inline void Poll() {
        // Wake one worker thread
        wakeCondition.notify_one();
        // Allow this thread to be rescheduled
        std::this_thread::yield();
    }

    void Execute(const std::function<void()>& job) {
        // The main thread label state is updated:
        currentLabel += 1;

        // Try to push a new job until it is pushed successfully:
        while (!jobPool.push_back(job)) {
            Poll();
        }

        wakeCondition.notify_one(); // wake one thread
    }

    bool IsBusy() {
        // Whenever the main thread label is not reached by the workers, it indicates that some worker is still alive
        return finishedLabel.load() < currentLabel;
    }

    void Wait() {
        while (IsBusy()) { Poll(); }
    }

    void Dispatch(uint32_t jobCount, uint32_t groupSize, const std::function<void(JobDispatchArgs)>& job) {
        if (jobCount == 0 || groupSize == 0) {
            return;
        }

        // Calculate the amount of job groups to dispatch (overestimate, or "ceil"):
        const uint32_t groupCount = (jobCount + groupSize - 1) / groupSize;

        // The main thread label state is updated:
        currentLabel += groupCount;

        for (uint32_t groupIndex = 0; groupIndex < groupCount; ++groupIndex) {
            // For each group, generate one real job:
            const auto& jobGroup = [jobCount, groupSize, job, groupIndex]() {

                // Calculate the current group's offset into the jobs:
                const uint32_t groupJobOffset = groupIndex * groupSize;
                const uint32_t groupJobEnd = std::min(groupJobOffset + groupSize, jobCount);

                JobDispatchArgs args {};
                args.groupIndex = groupIndex;

                // Inside the group, loop through all job indices and execute job for each index:
                for (uint32_t i = groupJobOffset; i < groupJobEnd; ++i) {
                    args.jobIndex = i;
                    job(args);
                }
            };

            // Try to push a new job until it is pushed successfully:
            while (!jobPool.push_back(jobGroup)) { Poll(); }

            wakeCondition.notify_one(); // wake one thread
        }
    }
}