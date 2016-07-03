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

bool ImageLayer::loadFromNode(pugi::xml_node const& layer)
{
    if (!layer)
    {
        return false;
    }
    if (!detail::LayerBase::loadFromNode(layer))
    {
        return false;
    }
    pugi::xml_node image = layer.child("image");
    if (!mImage.loadFromNode(image))
    {
        return false;
    }
    return loadImage();
}

void ImageLayer::saveToNode(pugi::xml_node& layer)
{
    if (!layer)
    {
        return;
    }
    detail::LayerBase::saveToNode(layer);
    pugi::xml_node image = layer.append_child("image");
    if (!image)
    {
        return;
    }
    mImage.saveToNode(image);
}

const std::string& ImageLayer::getSource() const
{
    return mImage.getSource();
}

const std::string& ImageLayer::getFormat() const
{
    return mImage.getFormat();
}

sf::Color ImageLayer::getTransparent() const
{
    return mImage.getTransparent();
}

const sf::Vector2u& ImageLayer::getSize() const
{
    return mImage.getSize();
}

void ImageLayer::setSource(std::string const& source)
{
    mImage.setSource(source);
}

void ImageLayer::setFormat(std::string const& format)
{
    mImage.setFormat(format);
}

void ImageLayer::setTransparent(sf::Color const& transparent)
{
    mImage.setTransparent(transparent);
}

void ImageLayer::setSize(sf::Vector2u const& size)
{
    mImage.setSize(size);
}

bool ImageLayer::loadImage()
{
    if (mImage.getSource() == "")
    {
        return false;
    }
    sf::Image img;
    if (!img.loadFromFile(mImage.getSource()))
    {
        return false;
    }
    if (!mTexture.loadFromImage(img))
    {
        return false;
    }
    float c = 255.f * mOpacity;
    mSprite.setColor(sf::Color(255,255,255,c));
    mSprite.setTexture(mTexture);
    return true;
}

void ImageLayer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (mVisible)
    {
        states.transform.translate(mOffset);
        target.draw(mSprite, states);
    }
}

} // namespace tmx
