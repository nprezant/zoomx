# Maintainer: Noah Prezant <noah.prezant@gmail.com>
pkgname=zoomx
pkgver=0.0.0
pkgrel=1
pkgdesc="Screen magnifying zoom utility for X11"
arch=('any')
url="https://github.com/boom-roasted/zoomx"
license=('MIT')
depends=(glibc libx11)
source=("main.c") # For AUR, will need something like: source=("https://github.com/boom-roasted/zoomx/archive/$pkgname-$pkgver.tar.gz
sha256sums=('cae0e757f5c906786ff18b4fb6f1406a4c4dfe7445a940af8ef620e6d33ddcd5')

build() {
	# For AUR, will need something like: cd "$pkgname-$pkgver"
	gcc -g main.c -L/usr/X11R6/lib -lX11 -o zoomx
}

package() {
	# For AUR, will need something like: cd "$pkgname-$pkgver"
	install -Dm751 zoomx "$pkgdir/usr/bin/zoomx"
}
