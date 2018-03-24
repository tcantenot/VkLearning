#ifndef CORE_APP_APP_HPP
#define CORE_APP_APP_HPP


class App
{
	public:
		App();
		~App();

		int run();

	private:
		virtual bool init() = 0;
		virtual void mainLoop() = 0;
		virtual void cleanup() = 0;
};

#endif //CORE_APP_APP_HPP
