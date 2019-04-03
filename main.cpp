#include <gtkmm.h>

int main(int argc, char *argv[])
{
  auto app =
    Gtk::Application::create(argc, argv,
      "org.gtkmm.examples.base");

  Gtk::Window* window;
  Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("ui1.glade");
  builder->get_widget("mainwin", window);

  return app->run(*window);
}
