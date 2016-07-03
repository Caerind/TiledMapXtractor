#include "ImageLayer.hpp"

namespace tmx
{

ImageLayer::ImageLayer()
{
}

LayerType ImageLayer::getLayerType() const
{
    return tmx::EImageLayer;
}

bool ImageLayer::loadFromNode(pugi::xml_node& layer)
{
    if (!detail::LayerBase::loadFromNode(layer))
    {
        return false;
    }
    pugi::xml_node image = layer.child("image");
    if (!mImage.loadFromNode(image))
    {
        return false;
    }
    std::string source = mImage.getSource();
    if (mImage.getSource() == "")
        return false;
    sf::Image img;
    if (!img.loadFromFile(source))
    {
        std::cerr << "Cant load image from source : " << source << std::endl;
        return false;
    }
    if (!mTexture.loadFromImage(img))
    {
        std::cerr << "Cant convert image to texture in " << mName << std::endl;
        return false;
    }
    float c = 255.f * mOpacity;
    mSprite.setColor(sf::Color(255,255,255,c));
    mSprite.setTexture(mTexture);
    return true;
}

void ImageLayer::saveToNode(pugi::xml_node& layer)
{
    detail::LayerBase::saveToNode(layer);
    pugi::xml_node image = layer.append_child("image");
    mImage.saveToNode(image);
}

void ImageLayer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (mVisible)
    {
        states.transform.translate(static_cast<sf::Vector2f>(mOffset));
        target.draw(mSprite, states);
    }
}

} // namespace tmx
