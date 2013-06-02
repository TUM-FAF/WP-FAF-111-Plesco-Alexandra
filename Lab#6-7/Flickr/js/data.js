/// <reference path="//Microsoft.WinJS.1.0/js/base.js" />
(function () {
    "use strict";

    var advices = new WinJS.Binding.List();

    var apiKey = '292dc483f3dd9da576dc08bec2ccf6e3';
    var userId = '41770947@N04';
    var apiUrl = 'http://api.flickr.com/services/rest/' +
        '?format=json&method=flickr.photos.search&' +
        'user_id=' + userId +
        '&sort=date-taken-desc' +
        '&api_key=' + apiKey +
        '&nojsoncallback=1';

    //var  = "http://fucking-great-advice.ru/api/latest/80";

    WinJS.xhr({ url: apiUrl }).then(function (xhr) {
        console.log(xhr.responseText);
        var items = JSON.parse(xhr.responseText);
        var photos = items.photos.photo;
        photos.forEach(function (item) {
            var photoURL = 'http://farm' + item.farm +
                '.static.flickr.com/' + item.server +
                '/' + item.id + '_' + item.secret;
            
            item.photoThumbnail = photoURL + '_s.jpg';
            item.photo = photoURL + '.jpg';

            advices.push(item);
        });
    });

    WinJS.Namespace.define("Data", {
        allAdvices: advices
    });

})();
