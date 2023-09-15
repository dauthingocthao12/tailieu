# How to use

Simply include `mobile_menu.js` and `mobile_menu.css` in the page.

The only things to be careful of, are the css class names (see below).

## Dependencies

- jQuery

## Sample HTML

```html
<!-- Mobile button to open the menu -->
<a class="mobile-menu-only mobile-menu-open-btn"><span>Menu</span></a>

<!-- 
	then the menu entries 
	this part will open on the right side on mobile
	and just be displayed as is on PC/tablet
	(no need of 2 different setups, unless different links/content is needed)
-->
<nav id="main-menu" class="container">

	<div class="mobile-menu-only">
		<a href="#close" class="mobile-menu-close-btn">Close</a>
		<div class="text-center">
			<a href="TOP.HTML" class="custom-logo-link" rel="home"><img src="LOGO.PNG" alt="COMPANY" /></a>
		</div>
	</div>

	<div class="menu-main-container">
		<ul class="content header-nav-main">
			<li><a href="/link1">PAGE 1</a></li>
			<li><a href="/link2">PAGE 2</a></li>
			<li><a href="/link3">PAGE 3</a></li>
			<li><a href="/link4">PAGE 4</a></li>
			<li><a href="/link5">PAGE 5</a></li>
			<li><a href="/link6">PAGE 6</a></li>
		</ul>
	</div>

	<div class="mobile-menu-only">
		<div class="text-center">
			<b>COMPANY</b>
			<address>
				ADDRESS
			</address>
			TEL&FAX: <a href="tel:XXX" class="mobile-tel-link">XXX</a>
			<br>
			Hours: XXX			
		</div>
	</div>
</nav>
```

# CSS classes meaning

## .mobile-menu-only

Element is only displayable on mobile (see break-point in CSS, customize if needed).

## .mobile-menu-open-btn

Element that will open the mobile menu.  
To be used together with `.mobile-menu-only`.

## .mobile-menu-close-btn

Element to close the menu, should be in the mobile menu so we can close it.

