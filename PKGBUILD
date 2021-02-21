# Maintainer: boom-roasted <noah dot prezant at gmail dot com>
pkgname=zoomx
pkgver=0.0.0
pkgrel=1
pkgdesc="Screen magnifying zoom utility for X11"
arch=('any')
url="https://github.com/boom-roasted/zoomx"
license=('MIT')
depends=(glibc libx11)
source=("zoomx.c")
sha256sums=('0569f4788ca95eacd2a1f617dc533bdefa4c8ec96085e71317f932bf5053c66a')

build() {
	gcc -g zoomx.c -L/usr/X11R6/lib -lX11 -o zoomx
}

package() {
	install -Dm751 zoomx "$pkgdir/usr/bin/zoomx"
}
