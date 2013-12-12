(function () {
  'use strict';
  var container = document.querySelector('#bricks');
  var msnry = new Masonry(container, {
    //options
    columnWidth: 50,
    itemSelector: '.post-head.group.small',
    gutter: 13.25
  })
}());