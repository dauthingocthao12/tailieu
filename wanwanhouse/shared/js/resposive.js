$(document).ready(function () {
  let navToggle = document.querySelector(".nav-toggle");
  let navWrapper = document.querySelector(".nav-wrapper");

  navToggle.addEventListener("click", function () {
    if (navWrapper.classList.contains("active")) {
      this.setAttribute("aria-expanded", "false");
      this.setAttribute("aria-label", "menu");
      navWrapper.classList.remove("active");
    } else {
      navWrapper.classList.add("active");
      this.setAttribute("aria-label", "close menu");
      this.setAttribute("aria-expanded", "true");
    }
  });
});
// var windowWidth = $(window).width();
//   $(window).resize(function () {
//     if (windowWidth <= 500) {
//       var width="290";
//     }else{
//       var width="340";
//     }
//     var src = "https://www.facebook.com/plugins/page.php?href=https%3A%2F%2Fwww.facebook.com%2FFacebook%2F&tabs=timeline&width="+width+"&height=500&small_header=true&adapt_container_width=true&hide_cover=false&show_facepile=false&appId"
//     $("#fbiframe").attr("src", "");
//     $("#fbiframe").attr("src", src);
//     $("#fbiframe").attr("width", width);
//     // $("#fbiframe").width(width);
//   });







// search-box open close js code
window.onload = function() {
  let navbar = document.querySelector(".navbar");
  let searchBox = document.querySelector(".search-box .bx-search");
  // let searchBoxCancel = document.querySelector(".search-box .bx-x");

  searchBox.addEventListener("click", ()=>{
    navbar.classList.toggle("showInput");
    if(navbar.classList.contains("showInput")){
      searchBox.classList.replace("bx-search" ,"bx-x");
    }else {
      searchBox.classList.replace("bx-x" ,"bx-search");
    }
  });

  // sidebar open close js code
  let navLinks = document.querySelector(".nav-links");
  let menuOpenBtn = document.querySelector(".navbar .bx-menu");
  let menuCloseBtn = document.querySelector(".nav-links .bx-x");
  menuOpenBtn.onclick = function() {
  navLinks.style.left = "0";
  }
  menuCloseBtn.onclick = function() {
  navLinks.style.left = "-100%";
  }


  // sidebar submenu open close js code
  let htmlcssArrow = document.querySelector(".htmlcss-arrow");
  htmlcssArrow.onclick = function() {
  navLinks.classList.toggle("show1");
  }
  let moreArrow = document.querySelector(".more-arrow");
  moreArrow.onclick = function() {
  navLinks.classList.toggle("show2");
  }
  let jsArrow = document.querySelector(".js-arrow");
  jsArrow.onclick = function() {
  navLinks.classList.toggle("show3");
  }
}
