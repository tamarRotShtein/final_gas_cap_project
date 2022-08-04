#include "handle_stages.h"
#include <unistd.h>
#include <stdlib.h>
void run_task(task * my_task)
{
   stage * stage=my_task->my_stage;
   do{
   if(stage->is_dequeue)
       my_task->my_current_node=dequeue(stage->sourseQu);
   stage->my_task(my_task);
   if(stage->is_enqueue)
       enqueu(stage->destQu,my_task->my_current_node);
   sleep(3);
   }
   while(stage->isActive);
   free(my_task);
}