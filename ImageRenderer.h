#pragma once
#include "Renderer.h"
#include "Vector2D.h"
#include "Image.h"

class ImageRenderer
	: virtual public Renderer
{
protected:
	Vector2D<int> position;
	Image* img;
	UINT transparent;
public:
	virtual void Render(HDC hdc) override;
	void SetPosition(int x, int y);
	void SetImg(Image* img);
	void SetTransparent(UINT rgb);
	ImageRenderer(Image* img, UINT transparent);
	ImageRenderer() {};
};