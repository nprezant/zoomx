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
sha256sums=('12a16b88afe8998e7c096e244f0aa3ac5ab6b014c4899dcf4d4790735deab2f5')

build() {
	gcc -g main.c -L/usr/X11R6/lib -lX11 -o zoomx
}

package() {
	install -Dm751 zoomx "$pkgdir/usr/bin/zoomx"
}
