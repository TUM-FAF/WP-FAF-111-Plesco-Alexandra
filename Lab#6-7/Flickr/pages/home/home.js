(function () {
    "use strict";

    WinJS.UI.Pages.define("/pages/home/home.html", {
        // This function is called whenever a user navigates to this page. It
        // populates the page elements with the app's data.
        ready: function (element, options) {
            element.querySelector("#visitSiteButton").addEventListener("click", function () {
                var siteUri = new Windows.Foundation.Uri('http://www.flickr.com/photos/41770947@N04/');

                Windows.System.Launcher.launchUriAsync(siteUri);
            });

            var adviceListView = element.querySelector("#adviceListView").winControl;
            adviceListView.oniteminvoked = this._itemInvoked.bind(this);
        },

        _itemInvoked: function (args) {
            var selectedItem = Data.allAdvices.getAt(args.detail.itemIndex);

            WinJS.Navigation.navigate("/pages/detail/detail.html",
                { item: selectedItem });
        }
    });
})();

// http://www.justinspradlin.com/programming/create-a-photostream-using-jquery-and-the-flickr-api/
// http://farm7.static.flickr.com/6179/6140716402_5640edd1a1.jpg
