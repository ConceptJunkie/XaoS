﻿#include <QtWidgets>

#include "config.h"
#include "filter.h"
#include "grlib.h"
#include "xio.h"

static QFont getFont(void *font) {
    if (font)
        return *reinterpret_cast<QFont *>(font);
    else
        return QFont(QApplication::font().family(), 12);
}

int xprint(struct image *image, void *font, int x, int y,
           const char *text, int fgcolor, int bgcolor, int mode)
{
    char line[BUFSIZ];
    int pos = strcspn(text, "\n");
    strncpy(line, text, pos);
    line[pos] = '\0';

    QImage *qimage = reinterpret_cast<QImage **>(image->data)[image->currimage];
    QFontMetrics metrics(getFont(font), qimage);
    QPainter painter(qimage);
    painter.setFont(getFont(font));

    if (mode == TEXT_PRESSED) {
        painter.setPen(fgcolor);
        painter.drawText(x + 1, y + 1 + metrics.ascent(), line);
    } else {
        painter.setPen(bgcolor);
        painter.drawText(x + 1, y + 1 + metrics.ascent(), line);
        painter.setPen(fgcolor);
        painter.drawText(x, y + metrics.ascent(), line);
    }

    return strlen(line);
}

int xtextwidth(struct image */*image*/, void *font, const char *text)
{
    char line[BUFSIZ];
    int pos = strcspn(text, "\n");
    strncpy(line, text, pos);
    line[pos] = '\0';

    QFontMetrics metrics(getFont(font));
    return metrics.width(line) + 1;
}

int xtextheight(struct image */*image*/, void *font)
{
    QFontMetrics metrics(getFont(font));
    return metrics.height() + 1;
}

int xtextcharw(struct image */*image*/, void *font, const char c)
{
    QFontMetrics metrics(getFont(font));
    return metrics.width(c);
}

const char *writepng(xio_constpath filename, const struct image *image)
{
    QImage *qimage = reinterpret_cast<QImage **>(image->data)[image->currimage];
    qimage->save(filename);
    return NULL;
}

static void freeImage(struct image *img)
{
    QImage **data = (QImage **)(img->data);
    delete data[0];
    delete data[1];
    delete data;
    free(img);
}

struct image *create_image_qt(int width, int height, struct palette *palette,
                              float pixelwidth, float pixelheight)
{
    QImage **data = new QImage *[2];
    data[0] = new QImage(width, height, QImage::Format_RGB32);
    data[1] = new QImage(width, height, QImage::Format_RGB32);
    struct image *img = create_image_cont(
        width, height, data[0]->bytesPerLine(), 2, data[0]->bits(),
        data[1]->bits(), palette, NULL, 0, pixelwidth, pixelheight);
    if (!img) {
        delete data[0];
        delete data[1];
        delete[] data;
        return NULL;
    }
    img->data = data;
    img->free = freeImage;
    return img;
}
