#ifndef file_dialog
#define file_dialog
#include <SFGUI/SFGUI.hpp>

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>
#include <memory>

using namespace sfg;
using namespace std;

class FileDialog
{
public:
    enum Action
    {
        NONE,
        SELECT_FILE,//open file, overwrite file
        SELECT_FOLDER,//get directory optionally with filename, save as
        SELECT_FOLDER_AND_TYPE_FILENAME
    };

    class Data
    {
        public:
        string directory;//    C:\Windows\System32
        string file_name;//    cmd.exe
        string file_path;//    C:\Windows\System32\cmd.exe

        Data(){}
        Data(string dir, string fn, string fp): directory(dir), file_name(fn), file_path(fp)  {}
    };
    Data data;
    FileDialog();
    void setDesktop(Desktop & desktop);
    void setRenderWindow(sf::RenderWindow & win);
    void update();
    void action(Action action_type);
    bool eventOccured();
    void enter_key_pressed();

private:

    Action a_type;
    bool event;
    string path;
    Desktop * desktop_;
    //shared_ptr<Desktop>desktop_;
    sf::RenderWindow * sfml_window;

    vector <string> folders, files;
    void listFiles(string path_name);

    class ExplorerItem;
    vector <ExplorerItem*> items;

    Window::Ptr dialog_window;
    Box::Ptr dialog_main_box;

    Box::Ptr top_box;                   //top
    Entry::Ptr path_entry;
    Button::Ptr go_parent_button;
    Button::Ptr go_path_button;
    void changeDirectory();
    void goParentDirectory();
    void restore_path();

    Box::Ptr explorer_box;                //middle
    ScrolledWindow::Ptr explorer_window;
    bool refresh_list;
    void fileClicked(const string & filename);

    Box::Ptr bottom_box;                //bottom
    Box::Ptr bottom_box_buttons;
    Entry::Ptr filename_entry;
    Button::Ptr apply, close;
    void apply_event(bool confirm);
    void return_ready();
    void hide_window();

    Window::Ptr info_window;            //info
    Box::Ptr info_box;
    Label::Ptr info_label;
    Button::Ptr info_button;
    void show_error(const string & message);
    void hide_error();

    Window::Ptr overwrite_window;            //overwrite
    Box::Ptr overwrite_box;
    Box::Ptr overwrite_box_b;
    Label::Ptr overwrite_label;
    Button::Ptr overwrite_button_ok;
    Button::Ptr overwrite_button_cancel;
    void show_overwrite_window(const string & message);
    void hide_overwrite_window();

private:
    class ExplorerItem
    {
        static sf::Image folder_icon, file_icon;

        Box::Ptr box;
        Image::Ptr folder_img, file_img;
        Label::Ptr label;

    public:
        bool to_delete;
        unsigned int connection_serial;

        ExplorerItem(int type, string name);
        static void loadImages();
        Box::Ptr get();
    };
};

#endif // file_dialog
