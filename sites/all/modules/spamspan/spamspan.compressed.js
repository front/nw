Drupal.behaviors.spamspan=function(_1){$("span."+Drupal.settings.spamspan.m,_1).each(function(_2){var _3=($("span."+Drupal.settings.spamspan.u,this).text()+"@"+$("span."+Drupal.settings.spamspan.d,this).text()).replace(/\s+/g,"").replace(/\[dot\]/g,".");var _4=$("span."+Drupal.settings.spamspan.h,this).text().replace(/^ ?\((.*)\) ?$/,"$1");var _5=$.map(_4.split(/, /),function(n,i){return (n.replace(/: /,"="));});var _6=$("span."+Drupal.settings.spamspan.t,this).text().replace(/^ \((.*)\)$/,"$1");var _7="mailto:"+_3;var _8=_5.join("&");_7+=_8?("?"+_8):"";$(this).after($("<a></a>").attr("href",_7).html(_6?_6:_3).addClass("spamspan")).remove();});};