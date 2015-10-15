#pragma once

namespace World
{
    class Shape;

    class World final
    {
    public:
		World();
		~World();

        const std::vector<std::unique_ptr<Shape>>& GetShapes() const { return _shapes; }
        std::vector<std::unique_ptr<Shape>>& GetShapes() { return _shapes; }

    private:
        std::vector<std::unique_ptr<Shape>> _shapes;
    };
}
