
class ExampleComponent : public SimComponent
{
public:
	ExampleComponent(SimComponentRegistry& registry) : SimComponent(registry, "ExampleComponent")
	{

	}

	coro::ReturnObject run() override
	{
		int i = 0;
		printf("ExampleComponent\n");
		while (1)
		{
			printf("loop-ExampleComponent: %d\n", i);
			co_await *this;
			i++;
		}
	}
};