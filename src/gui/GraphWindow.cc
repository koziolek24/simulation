#include "gui/GraphWindow.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <algorithm>
#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>

#include "SFML/System/Vector2.hpp"
#include "city/city.h"
#include "visitors/ShapeMakerVisitor.h"

void zpr::GraphWindow::autoFitScale()
{
    if (nodes_.empty())
        return;

    sf::Vector2f minPos = nodes_.front()->pos_;
    sf::Vector2f maxPos = minPos;

    for (const auto& n : nodes_) {
        minPos.x = std::min(minPos.x, n->pos_.x);
        minPos.y = std::min(minPos.y, n->pos_.y);
        maxPos.x = std::max(maxPos.x, n->pos_.x);
        maxPos.y = std::max(maxPos.y, n->pos_.y);
    }
    sf::Vector2f span = maxPos - minPos;

    const float halfW = view_ * 0.5f - view_ * 0.05f;
    const float sx = span.x > 0.f ? halfW / (span.x * 0.5f) : 1.f;
    const float sy = span.y > 0.f ? halfW / (span.y * 0.5f) : 1.f;
    scale_ = std::min(sx, sy);

    sf::Vector2f worldCentre = (minPos + maxPos) * 0.5f;
    offset_ = -worldCentre * scale_;
}

bool zpr::GraphWindow::init(const std::shared_ptr<SimulationEngine>& engine)
{
    engine_ = engine;
    nodes_ = engine->getAllNodes();
    size_t N = nodes_.size();
    float n = static_cast<float>(N);
    view_ = std::clamp(200.f + std::sqrt(n) * 50.f, 400.f, 1200.f);
    scale_ = std::clamp(1.2f + n * 0.005f, 1.0f, 10.0f);

    float spring = std::clamp(1.f + std::log(n + 1) * 2.f, 1.f, 10.f);
    float cooling = 0.96f;

    layout_ = std::make_unique<zpr::GraphLayout>(zpr::GraphLayout(view_ * scale_, spring, cooling));

    const float R0 = 30.f;
    for (unsigned int i = 0; i < nodes_.size(); ++i) {
        float a = (float)(i)*2.f * static_cast<float>(M_PI) / (float)(nodes_.size());
        nodes_[i]->pos_ = {std::cos(a) * R0, std::sin(a) * R0};
    }

    sf::Vector2f maxVel(10.f, 10.f);
    const float threshold = 0.01f;
    int iterations = 0;

    while (maxVel.x > threshold || maxVel.y > threshold) {
        layout_->iterate(nodes_);
        maxVel = sf::Vector2f(0.f, 0.f);

        for (const auto& node : nodes_) {
            maxVel.x = std::max(std::abs(node->vel_.x), maxVel.x);
            maxVel.y = std::max(std::abs(node->vel_.y), maxVel.y);
        }

        iterations++;
    }

    std::cout << "Layout stabilized after " << iterations << " iterations.\n"
              << maxVel.x << " " << maxVel.y << std::endl;

    autoFitScale();

    window_.create(sf::VideoMode({unsigned(view_), unsigned(view_)}), "Graph viewer");
    window_.setFramerateLimit(60);

#ifdef __APPLE__
    const char* fontPath_ = "/System/Library/Fonts/Supplemental/Arial.ttf";
#elif defined(__linux__)
    const char* fontPath_ = "/usr/share/fonts/gnu-free/FreeSans.ttf";
#elif defined(_WIN32)
    const char* fontPath_ = "C:/Windows/Fonts/arial.ttf";
#else
#error "Unsupported OS"
#endif

    if (!font_.loadFromFile(fontPath_))
        std::cerr << "Embedded font failed to load\n";

    stats_.emplace("", font_, 14u);
    stats_.value().setFont(font_);
    stats_.value().setCharacterSize(14u);
    stats_.value().setFillColor(sf::Color::Black);
    stats_.value().setOutlineColor(sf::Color::White);
    stats_.value().setOutlineThickness(2.f);
    stats_.value().setString("Nodes: " + std::to_string(nodes_.size()));

    return true;
}

int zpr::GraphWindow::run()
{
    if (!window_.isOpen())
        return -1;

    while (window_.isOpen()) {
        layout_->iterate(nodes_);
        processEvents();
        if (!window_.isOpen())
            break;
        render();
    }
    return 0;
}

void zpr::GraphWindow::processEvents()
{
    sf::Event ev;
    while (window_.pollEvent(ev)) {
        if (ev.type == sf::Event::Closed) {
            window_.close();
        }

        if (ev.type == sf::Event::KeyPressed) {
            const float step = 20.f;
            switch (ev.key.code) {
                case sf::Keyboard::Key::Left:
                    offset_.x += step;
                    break;
                case sf::Keyboard::Key::Right:
                    offset_.x -= step;
                    break;
                case sf::Keyboard::Key::Up:
                    offset_.y += step;
                    break;
                case sf::Keyboard::Key::Down:
                    offset_.y -= step;
                    break;
                case sf::Keyboard::Key::Hyphen:
                    scale_ *= 0.9f;
                    break;
                case sf::Keyboard::Key::Equal:
                    scale_ *= 1.1f;
                    break;
                case sf::Keyboard::Key::Escape:
                    current_.reset();
                    break;
                case sf::Keyboard::Key::Enter:
                    autoFitScale();
                    break;
                default:
                    break;
            }
        }

        if (ev.type == sf::Event::MouseButtonPressed &&
            ev.mouseButton.button == sf::Mouse::Button::Left) {
            sf::Vector2f mp{float(ev.mouseButton.x), float(ev.mouseButton.y)};
            sf::Vector2f center{view_ / 2.f, view_ / 2.f};
            current_.reset();
            for (auto& n : nodes_) {
                sf::Vector2f temp = mp - (n->pos_ * scale_ + center + offset_);
                if (std::hypot(temp.x, temp.y) < radius_) {
                    current_ = std::weak_ptr<Node>(n);
                }
            }
        }
    }
}

void zpr::GraphWindow::render()
{
    uint allPeopleCount{0};
    uint infectedPeopleCount{0};
    window_.clear(sf::Color::White);
    sf::Vector2f center{view_ / 2.f, view_ / 2.f};

    std::array<sf::Vertex, 2> line;
    line[0].color = line[1].color = sf::Color::Black;

    for (auto& n : nodes_)
        for (auto& w : n->getAllNeighbours()) {
            line[0].position = n->pos_ * scale_ + center + offset_;
            line[1].position = w.lock()->pos_ * scale_ + center + offset_;
            window_.draw(line.data(), 2, sf::PrimitiveType::Lines);
        }

    for (auto& n : nodes_) {
        zpr::ShapeMakerVisitor shapeMakerVisitor(radius_);
        n->accept(shapeMakerVisitor);
        sf::Shape& shp = shapeMakerVisitor.getShape();
        float ratio{0};

        allPeopleCount += n->getPeopleCount();
        infectedPeopleCount += n->getInfectedCount();

        if (n->getPeopleCount() > 0) {
            ratio = std::clamp(
                static_cast<float>(n->getInfectedCount()) / static_cast<float>(n->getPeopleCount()),
                0.f, 1.0f);
            sf::Uint8 r = static_cast<sf::Uint8>(128 + ratio * (255 - 128));
            sf::Uint8 g = static_cast<sf::Uint8>(128 * (1 - ratio));
            sf::Uint8 b = static_cast<sf::Uint8>(128 * (1 - ratio));
            shp.setFillColor(sf::Color(r, g, b));
        }
        else {
            shp.setFillColor(sf::Color::White);
        }

        shp.setOutlineThickness(2.f);
        shp.setOutlineColor(sf::Color::Black);
        shp.setPosition(n->pos_ * scale_ + center + offset_);
        window_.draw(shp);
    }
    if (current_.lock()) {
        sf::RectangleShape bg({view_ * 0.6f, view_ * 0.35f});
        bg.setFillColor(sf::Color(0, 0, 0, 180));
        bg.setOrigin(bg.getSize() / 2.f);
        bg.setPosition({view_ / 2.f, view_ / 2.f});
        window_.draw(bg);

        std::string txt =
            "ID   : " + current_.lock()->getName() + '\n' +
            "People   : " + std::to_string(current_.lock()->getPeopleCount()) + '\n' +
            "Healthy   : " + std::to_string(current_.lock()->getHealthyCount()) + '\n' +
            "Infected   : " + std::to_string(current_.lock()->getInfectedCount()) + '\n' +
            "Edges: " + std::to_string(current_.lock()->getAllNeighbours().size());
        sf::Text info(txt, font_, 18u);
        info.setFillColor(sf::Color::White);
        auto bb = info.getLocalBounds();
        info.setOrigin({bb.width / 2.f, bb.height / 2.f});
        info.setPosition(bg.getPosition());
        window_.draw(info);
    }
    if (stats_) {
        std::ostringstream oss;
        oss << "Day: " << engine_->getDay() << " " << std::setw(2) << std::setfill('0')
            << engine_->getHour() << ":" << std::setw(2) << std::setfill('0')
            << engine_->getMinute() << "\n"
            << "Nodes: " << nodes_.size() << "\n"
            << "People: " << allPeopleCount << "\n"
            << "Infected: " << infectedPeopleCount << "\n";
        stats_.value().setString(oss.str());
        stats_.value().setPosition({view_ - stats_.value().getGlobalBounds().width - 8.f, 8.f});
        window_.draw(stats_.value());
    }
    window_.display();
}
