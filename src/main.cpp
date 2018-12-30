#include "utils/mouse.hpp"
#include "utils/parameters.hpp"
#include "geometry/camera3d.hpp"
#include "asteroids_field.hpp"

using std::cout;
using std::endl;


int main()
{
    sf::Clock loop_timer;
    sf::Clock current_time;

	srand(time(nullptr));


    // setup window
    sf::ContextSettings window_settings;
    window_settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT), "3D-engine", sf::Style::Close | sf::Style::Resize, window_settings);
    window.setVerticalSyncEnabled(true);
    window.setKeyRepeatEnabled(false);
    Mouse::setPosition(sf::Vector2i(INITIAL_WINDOW_WIDTH / 2, INITIAL_WINDOW_HEIGHT / 2), window);  

    // create camera
    Camera3d camera(Vector3d(0, -120, -230), -30, 0, 0, Parameters::window_width, Parameters::window_height);

    // create asteroids field
    AsteroidsField field(camera.get_position(), 100, 500);


    loop_timer.restart();
    current_time.restart();
    

    while (window.isOpen())
    {
        sf::Event event;

        // handle events
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();
            if (event.type == sf::Event::Resized) {
                window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
                Parameters::update_window_size(event.size.width, event.size.height);
                cout << "Window resized: "
                     << event.size.width
                     << " x "
                     << event.size.height << endl;
                camera.reload_frustrum(Parameters::window_width, Parameters::window_height);
            }
        }

        // rotate camera
        camera.rotate(Mouse::get_move_x(window), Mouse::get_move_y(window));

        // move camera
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
            camera.move(Camera3d::DIRECTION::FRONT);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            camera.move(Camera3d::DIRECTION::BACK);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            camera.move(Camera3d::DIRECTION::RIGHT);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            camera.move(Camera3d::DIRECTION::LEFT);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            camera.move(Camera3d::DIRECTION::UP);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::E))
            camera.move(Camera3d::DIRECTION::DOWN);

        // rendering
        window.clear();

        field.move_objects();
        field.render_objects(window, Parameters::window_width, Parameters::window_height, camera);

        window.display();

        // other
        Parameters::print_mean_CPU_usage(cout, loop_timer.getElapsedTime().asMilliseconds());

        sf::sleep(sf::milliseconds(MAX_MAIN_LOOP_DURATION - loop_timer.getElapsedTime().asMilliseconds()));
        loop_timer.restart();
    }

    cout << endl;
    
	return 0;
}
