# Maintainer: Noah Prezant <noah dot prezant at gmail dot com>
pkgname=zoomx
pkgver=0.0.0
pkgrel=1
pkgdesc="Screen magnifying zoom utility for X11"
arch=('any')
url="https://github.com/boom-roasted/zoomx"
license=('MIT')
depends=(glibc libx11)
source=("main.c")
sha256sums=('ffcad9706766ff608123151104323c96df1f0e75deaec74a428889af8c940ecc')

build() {
	gcc -g main.c -L/usr/X11R6/lib -lX11 -o zoomx
}

package() {
	install -Dm751 zoomx "$pkgdir/usr/bin/zoomx"
}
