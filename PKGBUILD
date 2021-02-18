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
sha256sums=('8e9d9fe1dd018daedff535af6c33e5f4b54988f65e8fea865d91a1b545c8f6b2')

build() {
	gcc -g main.c -L/usr/X11R6/lib -lX11 -o zoomx
}

package() {
	install -Dm751 zoomx "$pkgdir/usr/bin/zoomx"
}
