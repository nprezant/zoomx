# Maintainer: Noah Prezant <noah.prezant@gmail.com>
pkgname=zoomx
pkgver=0.0.0
pkgrel=1
pkgdesc="Screen magnifying zoom utility for X11"
arch=('any')
url="https://github.com/boom-roasted/zoomx"
license=('MIT')
depends=(glibc libx11)
source=("main.c"
        "${pkgname}.desktop") # For AUR, will need something like: source=("https://github.com/boom-roasted/zoomx/archive/$pkgname-$pkgver.tar.gz
sha256sums=('eeb464fe349161edc4ef4131c257c743421efbd354d3d9a14482de458246a166'
            '87b87a33598faea694d732d17ddd1ec51776b78303975cf7a70459caefe5fe92')

build() {
	# For AUR, will need something like: cd "$pkgname-$pkgver"
	gcc -g main.c -L/usr/X11R6/lib -lX11 -o zoomx
}

package() {
	# For AUR, will need something like: cd "$pkgname-$pkgver"
	install -Dm751 zoomx "$pkgdir/usr/bin/zoomx"
	install -Dm644 "${pkgname}.desktop" "${pkgdir}/usr/share/applications/${pkgname}.desktop"
}
