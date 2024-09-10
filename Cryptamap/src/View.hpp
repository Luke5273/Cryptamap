

class View
{
public:
	View() {};
	~View();
	View(const View& obj) = delete;
	static View* getInstance();

	void init();

private:
	static View* m_self;
};

