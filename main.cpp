#include "FileDialog/FileDialog.hpp"

class Gui
{
    sfg::Box::Ptr box;
    sfg::Window::Ptr window;
    sfg::Label::Ptr label;

    //buttons
    sfg::Button::Ptr new_file_button;
    sfg::Button::Ptr open_file_button;
    sfg::Button::Ptr open_dir_button;
    sfg::Button::Ptr save_file_as_button;

    //actions for buttons
    enum GuiOperations
    {
        NONE,
        OPEN_FILE,          //open_file button
        SELECT_DIRECTORY,   //open_dir button
        CREATE_NEW_FILE,    //new_file button
        SAVE_AS,            //save_file_as button
    };
    GuiOperations my_operation;

    FileDialog filedialog;      //creating file dialog

    public:
    Gui(sfg::Desktop & desktop, sf::RenderWindow & renderwindow)
    {
        filedialog.setDesktop(desktop);
        filedialog.setRenderWindow(renderwindow);

        window = sfg::Window::Create();
        box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
        window->Add(box);
        desktop.Add(window);
        label = sfg::Label::Create("\nDo something");

        new_file_button = sfg::Button::Create("New");
        new_file_button->GetSignal( sfg::Button::OnLeftClick ).Connect(bind( &Gui::create_new_file, this));
        open_file_button = sfg::Button::Create("Open");
        open_file_button->GetSignal( sfg::Button::OnLeftClick ).Connect(bind( &Gui::open_file, this));
        open_dir_button = sfg::Button::Create("Open directory");
        open_dir_button->GetSignal( sfg::Button::OnLeftClick ).Connect(bind( &Gui::open_dir, this));
        save_file_as_button = sfg::Button::Create("Save as");
        save_file_as_button->GetSignal( sfg::Button::OnLeftClick ).Connect(bind( &Gui::save_file_as, this));

        box->Pack(new_file_button);
        box->Pack(open_file_button);
        box->Pack(open_dir_button);
        box->Pack(save_file_as_button);
        box->Pack(label);
    }

    FileDialog & get_filedialog()
    {
        return filedialog;
    }

    void update()
    {
        filedialog.update();

        if(filedialog.eventOccured())
        {
            if(my_operation == OPEN_FILE)
            {
                cout<<"Open file: "<<filedialog.data.file_path<<endl;
                label->SetText("\nOpen file: \n"+filedialog.data.file_path);
            }

            if(my_operation == CREATE_NEW_FILE)
            {
                cout<<"Create new file: "<<filedialog.data.file_name<<" in "<<filedialog.data.directory<<endl;
                label->SetText("\nCreate file: \n"+filedialog.data.file_name+" in "+filedialog.data.directory+"\n("+filedialog.data.file_path+")");
            }

            if(my_operation == SELECT_DIRECTORY)
            {
                cout<<"Select directory: "<<filedialog.data.directory<<endl;
                label->SetText("\nSelect directory:\n"+filedialog.data.directory);
            }

            if(my_operation == SAVE_AS)
            {
                cout<<"Save as: "<<filedialog.data.file_path<<endl;
                label->SetText("\nSave as:\n"+filedialog.data.file_path);
            }

            my_operation = NONE;
        }
        //filedialog.eventOccured() here will return false beacuse it was already called
    }

    private:

    void create_new_file()
    {
        cout<<"create_new_file()"<<endl;
        my_operation = CREATE_NEW_FILE;
        filedialog.action(FileDialog::Action::SELECT_FOLDER_AND_TYPE_FILENAME);
    }

    void open_file()
    {
        cout<<"open_file()"<<endl;
        my_operation = OPEN_FILE;
        filedialog.action(FileDialog::Action::SELECT_FILE);
    }

    void open_dir()
    {
        cout<<"open_dir()"<<endl;
        my_operation = SELECT_DIRECTORY;
        filedialog.action(FileDialog::Action::SELECT_FOLDER);
    }

    void save_file_as()
    {
        cout<<"save_file_as()"<<endl;
        my_operation = SAVE_AS;
        filedialog.action(FileDialog::Action::SELECT_FOLDER_AND_TYPE_FILENAME);
    }

};

int main()
{
	sf::RenderWindow sfmlwindow( sf::VideoMode( 800, 600 ), "FileDialog example");
	sfmlwindow.resetGLStates();

	sfg::SFGUI m_sfgui;
	sfg::Desktop desktop;
	Gui my_gui(desktop, sfmlwindow);

    sf::Event event;
    sf::Clock clock;
    clock.restart();
	while( sfmlwindow.isOpen() )
    {
		while( sfmlwindow.pollEvent( event ) )
        {
			desktop.HandleEvent( event );
			if( event.type == sf::Event::Closed )
				sfmlwindow.close();
            else if( event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return )
                my_gui.get_filedialog().enterKeyPressed();
		}

		my_gui.update();

		desktop.Update( clock.restart().asSeconds() );
        sfmlwindow.clear();
		m_sfgui.Display( sfmlwindow );
		sfmlwindow.display();
    }
}
