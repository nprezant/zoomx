# Maintainer: Noah Prezant <noah.prezant@gmail.com>
pkgname=zoomx
pkgver=0.0.0
pkgrel=1
pkgdesc="Screen magnifying zoom utility for X11"
arch=('any')
url="https://github.com/boom-roasted/zoomx"
license=('MIT')
depends=(glibc libx11)
source=("main.c")
sha256sums=('eeb464fe349161edc4ef4131c257c743421efbd354d3d9a14482de458246a166')

build() {
	gcc -g main.c -L/usr/X11R6/lib -lX11 -o zoomx
}

package() {
	install -Dm751 zoomx "$pkgdir/usr/bin/zoomx"
}
