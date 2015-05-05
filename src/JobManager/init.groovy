// JENKINS_HOME/init.groovy
// Run at Jenkins startup
//   (can also execute at /script after Jenkins is running)
// Requires:
//  https://wiki.jenkins-ci.org/display/JENKINS/Priority+Sorter+Plugin
// Known working plugin versions: 2.6

// VehicleForge allows users to mark a job "high-priority", which means that it should skip non-high-priority jobs in the queue
// The Jenkins "Priority Sorter Plugin" allows jobs to have a priority, but the priority is not read from the project config once the job is queued
// This file implements a new QueueSorter which re-reads the priority from the project config when sorting the queue

// HOWTO log:
//  visit /log/, create a new log recorder, Logger: OnlinePrioritySorter

// WARNING: the Priority Sorter Plugin Strategy option under /configure is ignored
// WARNING: default priority of 3 is assumed

import hudson.model.*;
import java.util.logging.Logger;
import static java.util.logging.Level.*;
import hudson.model.Queue.Task;
import hudson.model.queue.MappingWorksheet;
import hudson.model.queue.MappingWorksheet.ExecutorChunk;
import hudson.model.queue.MappingWorksheet.Mapping;

import jenkins.advancedqueue.AdvancedQueueSorterJobProperty;

import hudson.model.queue.QueueSorter;

public class PriorityLoadBalancer extends LoadBalancer {
    public static final Logger LOGGER = Logger.getLogger(PriorityLoadBalancer.class.getCanonicalName());

    private final LoadBalancer fallback = LoadBalancer.CONSISTENT_HASH;
    @Override
    public Mapping map(Task task, MappingWorksheet ws) {
        LOGGER.log(WARNING, task.toString());
        return fallback.map(task, ws);
    }
}

public class OnlinePrioritySorter extends QueueSorter implements Comparator<hudson.model.Queue.BuildableItem> {
    public static final Logger LOGGER = Logger.getLogger(OnlinePrioritySorter.class.getCanonicalName());

    @Override
    public void sortBuildableItems(List<hudson.model.Queue.BuildableItem> buildables) {
    
        Collections.sort(buildables, new Comparator<hudson.model.Queue.BuildableItem>() {
            public int compare(hudson.model.Queue.BuildableItem o1, hudson.model.Queue.BuildableItem o2) {
                return (int) (o1.getInQueueSince() - o2.getInQueueSince());
            }
        });

        Collections.sort(buildables, this);
        LOGGER.log(FINER, "sortBuildableItems completed");
    }

    public int compare(hudson.model.Queue.BuildableItem lhs, hudson.model.Queue.BuildableItem rhs) {
        if (!(lhs.task instanceof hudson.model.Project) || !(rhs.task instanceof hudson.model.Project))
        {
            LOGGER.log(FINEST, "Won't compare non-Projects: " + lhs.getClass().getName() + " " + rhs.getClass().getName());
            return 0;
        }
       
        AdvancedQueueSorterJobProperty lhsProp = 
              (AdvancedQueueSorterJobProperty)lhs.task.getProperty(AdvancedQueueSorterJobProperty);
        AdvancedQueueSorterJobProperty rhsProp = 
              (AdvancedQueueSorterJobProperty)rhs.task.getProperty(AdvancedQueueSorterJobProperty);

        int lhsPriority = lhsProp == null ? 3 : lhsProp.getPriority();
        int rhsPriority = rhsProp == null ? 3 : rhsProp.getPriority();
              
        LOGGER.log(FINEST, lhs.task.getDisplayName() + " " + lhsPriority + " : " + rhs.task.getDisplayName() + " " + rhsPriority);
        return (int) (lhsPriority - rhsPriority);
    }
}


OnlinePrioritySorter.LOGGER.log(WARNING, "OnlinePrioritySorter initializing");
Hudson.getInstance().getQueue().setSorter(new OnlinePrioritySorter());

//Hudson.getInstance().getQueue().setLoadBalancer(new PriorityLoadBalancer());

// start in the state that doesn't do any build.
// Hudson.instance.doQuietDown();
