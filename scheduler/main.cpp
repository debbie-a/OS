#include "scheduler.h"
#include "Tasks/eat_chocolate.h"
#include "Tasks/play_music.h"
#include "Tasks/play_games.h"
#include "Tasks/go_shopping.h"
#include "Tasks/go_swimming.h"
#include "Tasks/clean_house.h"
#include "Tasks/home_work.h"
#include "Tasks/bake_cakes.h"

int main()
{
	Scheduler scheduler;

    	SharedPtr<ITask> task1(new EatChocolate(0, 13, 500000));
    	SharedPtr<ITask> task2(new PlayMusic(200, 10, 500001));
    	SharedPtr<ITask> task3(new PlayGames(400, 12, 500002));
	SharedPtr<ITask> task4(new GoShopping(600, 20, 500003));
	SharedPtr<ITask> task5(new GoSwimming(800, 8, 500004));
	SharedPtr<ITask> task6(new HomeWork(1000, 4, 500005));
	SharedPtr<ITask> task7(new BakeCakes(1200, 15, 500006));

    	
	scheduler.addTask(task1);
    	scheduler.addTask(task2);
    	scheduler.addTask(task3);
	scheduler.addTask(task4);
	scheduler.addTask(task5);
	scheduler.addTask(task6);
	scheduler.addTask(task7);

    	scheduler.run();

    	return 0;
}
