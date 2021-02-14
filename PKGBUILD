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
sha256sums=('51638dcb10b7ac7b710238bdbc5d1b6288c285535c118585e47fa063a6c20de0')

build() {
	gcc -g main.c -L/usr/X11R6/lib -lX11 -o zoomx
}

package() {
	install -Dm751 zoomx "$pkgdir/usr/bin/zoomx"
}
