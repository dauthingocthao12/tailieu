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

  var first_width = $(window).width();
  $(window).resize(function () {
    if (first_width <= 500) {
      console.log("AAAA");
      var width="290";
    }else{
      var width="340";
    }
    var src = "https://www.facebook.com/plugins/page.php?href=https%3A%2F%2Fwww.facebook.com%2FFacebook%2F&tabs=timeline&width="+width+"&height=500&small_header=true&adapt_container_width=true&hide_cover=false&show_facepile=false&appId"
    $("#fbiframe").attr("src", "");
    $("#fbiframe").attr("src", src);
    $("#fbiframe").attr("width", width);
  });



  // fbiframe_reload();
//   jQuery(function($){
//     var first_width = $(window).width();
     
//     var timer = false;
//     $(window).resize(function() {
//         if (timer !== false) {
//             clearTimeout(timer);
//         }
//         timer = setTimeout(function() {
//             //resize完了時の動作
//             var width = $(window).width();
//             if(width!=first_width){
     
//                 //console.log('resized');
//                 fbiframe_reload();
//                 first_width=width;
//             }
//         }, 200);
//     });
     
//     function fbiframe_reload(){//facebookウィジェットの再描画
     
//             var width = $(".facebook").width();//親要素のID,class要変更
     
//             var src = "https://www.facebook.com/plugins/page.php?href=https%3A%2F%2Fwww.facebook.com%2FFacebook%2F&tabs=timeline&width="+width+"&height=500&small_header=true&adapt_container_width=true&hide_cover=false&show_facepile=false&appId"
     
//             $("#fbiframe").attr("src","");       
//             $("#fbiframe").attr("src",src); 
     
//             $("#fbiframe").attr("width",width); 
//     };
     
//     fbiframe_reload();
//     });