#include "AComponent.hpp"

#pragma once

namespace ypi::gui
{
    class Image : virtual public AComponent
    {
    public:
        Image(const sf::Texture&, const Vector2f& = Vector2f(0, 0));
        ~Image() = default;

        void render(RenderTexture& target) override;
        FloatRect getGlobalBounds() const override;

    protected:
        virtual void imp_setPosition() override;
    };
}
