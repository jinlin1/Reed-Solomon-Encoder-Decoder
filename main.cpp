#include <gtkmm.h>
#include <iostream>

void encode_button_clicked(Glib::RefPtr<Gtk::Builder> builder)
{
  Gtk::ComboBoxText* gf;
  Gtk::ComboBoxText* prim_poly;
  Gtk::TextView* msg;
  builder->get_widget("GF Size", gf);
  builder->get_widget("Prim Poly", prim_poly);
  builder->get_widget("msg", msg);

  std::cout << gf->get_active_text() << "\n";
  std::cout << prim_poly->get_active_id() << "\n";
  std::cout << msg->get_buffer()->get_text() << "\n";

}


int main(int argc, char *argv[])
{
  auto app =
    Gtk::Application::create(argc, argv,
      "org.gtkmm.examples.base");

  Gtk::Window* window;
  Gtk::Button* encodeButton;
  Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("ui1.glade");
  builder->get_widget("mainwin", window);
  builder->get_widget("encode", encodeButton);
  encodeButton->signal_clicked().connect(sigc::bind<Glib::RefPtr<Gtk::Builder>>(sigc::ptr_fun(&encode_button_clicked), builder));

  return app->run(*window);
}
