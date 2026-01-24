<?php

function sharebtn($url, $raw_url = '')
{
	$out = '<!-- Share button by NiftyButtons - Visit https://www.niftybuttons.com -->
		<button 
    onclick="document.getElementById(\'popup_\').style.display=\'flex\'"
    style="background-color:#986a44; color:#ffffff; padding: 10px 16px; border: none; border-radius: 5px; cursor: pointer; font-size: 16px;">
    <svg xmlns="http://www.w3.org/2000/svg" width="15px" height="15px" viewBox="0 0 15 15" style="fill:#ffffff; margin-right: 5px; vertical-align: middle;"><path d="M 8.992188 1.019531 C 8.65625 1.167969 8.4375 1.507812 8.4375 1.875 L 8.4375 3.75 L 5.15625 3.75 C 2.308594 3.75 0 6.058594 0 8.90625 C 0 12.226562 2.386719 13.707031 2.9375 14.007812 C 3.007812 14.046875 3.089844 14.0625 3.171875 14.0625 C 3.492188 14.0625 3.75 13.800781 3.75 13.484375 C 3.75 13.265625 3.625 13.0625 3.460938 12.914062 C 3.1875 12.652344 2.8125 12.140625 2.8125 11.25 C 2.8125 9.695312 4.070312 8.4375 5.625 8.4375 L 8.4375 8.4375 L 8.4375 10.3125 C 8.4375 10.679688 8.65625 11.019531 8.992188 11.167969 C 9.335938 11.316406 9.726562 11.257812 10 11.007812 L 14.6875 6.789062 C 14.886719 6.613281 15 6.359375 15 6.09375 C 15 5.828125 14.890625 5.574219 14.6875 5.398438 L 10 1.175781 C 9.726562 0.929688 9.332031 0.867188 8.992188 1.019531 Z M 8.992188 1.019531 "/></svg>Jaa t&auml;m&auml setti!</button><div id="popup_" style=" position: fixed; top: 0; left: 0; right: 0; bottom: 0; background-color: rgba(0,0,0,0.5); display: none; justify-content: center; align-items: center;">
                
        <div style="background-color: #fff; z-index: 99; padding:20px; border-radius: 10px; width: 340px; box-shadow: 0px 3px 10px rgba(0,0,0,0.2); position: relative;">
            <span onclick="document.getElementById(\'popup_\').style.display=\'none\'" style="position: absolute; right: 10px; top: 10px; cursor: pointer; width: 24px; height: 24px; display: flex; align-items: center; justify-content: center;" id="closeBtn">
                <svg xmlns="http://www.w3.org/2000/svg" width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="#333" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                    <line x1="18" y1="6" x2="6" y2="18"></line>
                    <line x1="6" y1="6" x2="18" y2="18"></line>
                </svg>
            </span>
            <div id="share header" style="font-weight: bold; padding-bottom:10px; text-align: center; color:#000000 !important;">Share</div>
            <ul style="display: flex; flex-wrap: wrap; justify-content: space-between; list-style: none; padding: 0;">
    
        <li style="width: 48%; margin-bottom: 10px; font-size: 16px; cursor: pointer;" onclick="(function() { 
	   /* var link = \''.$raw_url.'\'; */
	      var link = getShareUrl();

	    navigator.clipboard.writeText(link).then(
		() => {
		        alert(\'successfully copied\');
      		}).catch(() => {
        alert(\'something went wrong\');
      }
		/*function() { 
                alert(\'URL copied to clipboard!\'); 
	    }
).catch(function(err) {
                console.error(\'Could not copy URL\', err);
	    }
		*/
		);
         })()">
         <svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" style="display:flex; float:left; margin-right: 10px;" fill="#cdab8f">
        <path d="M13.723 18.654l-3.61 3.609c-2.316 2.315-6.063 2.315-8.378 0-1.12-1.118-1.735-2.606-1.735-4.188 0-1.582.615-3.07 1.734-4.189l4.866-4.865c2.355-2.355 6.114-2.262 8.377 0 .453.453.81.973 1.089 1.527l-1.593 1.592c-.18-.613-.5-1.189-.964-1.652-1.448-1.448-3.93-1.51-5.439-.001l-.001.002-4.867 4.865c-1.5 1.499-1.5 3.941 0 5.44 1.517 1.517 3.958 1.488 5.442 0l2.425-2.424c.993.284 1.791.335 2.654.284zm.161-16.918l-3.574 3.576c.847-.05 1.655 0 2.653.283l2.393-2.389c1.498-1.502 3.94-1.5 5.44-.001 1.517 1.518 1.486 3.959 0 5.442l-4.831 4.831-.003.002c-1.438 1.437-3.886 1.552-5.439-.002-.473-.474-.785-1.042-.956-1.643l-.084.068-1.517 1.515c.28.556.635 1.075 1.088 1.528 2.245 2.245 6.004 2.374 8.378 0l4.832-4.831c2.314-2.316 2.316-6.062-.001-8.377-2.317-2.321-6.067-2.313-8.379-.002z"></path></svg>
         <span style="color:#000000 !important;"><span style="margin-left: 10px;">Kopioi linkki</span></span>
	 </li>

            <li style="width: 48%; font-size: 16px; margin-bottom: 10px;">
            <a id="mailto-share-link" href="mailto:?subject=Kokeilemani Dominionkortit&amp;body='.$url.'" style="display: flex; align-items: center; text-decoration: none; color:#000000 !important;">
        <svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" style="margin-right: 10px;" fill="#cdab8f"><path d="M20 4H4a2 2 0 00-2 2v12a2 2 0 002 2h16a2 2 0 002-2V6a2 2 0 00-2-2zm0 4l-8 5-8-5V6l8 5 8-5v2z"></path></svg> <!-- directly use the SVG string -->
        <span style="margin-left: 10px;color:#000000 !important;">Email</span>
        </a>
        </li>
            <li style="width: 48%; font-size: 16px; margin-bottom: 10px;">
            <a id="tumblr-share-link" href="https://www.tumblr.com/widgets/share/tool?canonicalUrl='.$url.'" style="display: flex; align-items: center; text-decoration: none; color:#000000 !important;">
        <svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" style="margin-right: 10px;" fill="#cdab8f"><path d="M19.512 17.489l-.096-.068h-3.274c-.153 0-.16-.467-.163-.622v-5.714c0-.056.045-.101.101-.101h3.82c.056 0 .101-.045.101-.101v-5.766c0-.055-.045-.1-.101-.1h-3.803c-.055 0-.1-.045-.1-.101v-4.816c0-.055-.045-.1-.101-.1h-7.15c-.489 0-1.053.362-1.135 1.034-.341 2.778-1.882 4.125-4.276 4.925l-.267.089-.068.096v4.74c0 .056.045.101.1.101h2.9v6.156c0 4.66 3.04 6.859 9.008 6.859 2.401 0 5.048-.855 5.835-1.891l.157-.208-1.488-4.412zm.339 4.258c-.75.721-2.554 1.256-4.028 1.281l-.165.001c-4.849 0-5.682-3.701-5.682-5.889v-7.039c0-.056-.045-.101-.1-.101h-2.782c-.056 0-.101-.045-.101-.101l-.024-3.06.064-.092c2.506-.976 3.905-2.595 4.273-5.593.021-.167.158-.171.159-.171h3.447c.055 0 .101.045.101.101v4.816c0 .056.045.101.1.101h3.803c.056 0 .101.045.101.1v3.801c0 .056-.045.101-.101.101h-3.819c-.056 0-.097.045-.097.101v6.705c.023 1.438.715 2.167 2.065 2.167.544 0 1.116-.126 1.685-.344.053-.021.111.007.13.061l.995 2.95-.024.104z" fill-rule="evenodd" clip-rule="evenodd"></path></svg> <!-- directly use the SVG string -->
        <span style="margin-left: 10px;color:#000000 !important;">Tumblr</span>
        </a>
        </li>
            <li style="width: 48%; font-size: 16px; margin-bottom: 10px;">
            <a id="facebook-share-link" href="https://facebook.com/sharer.php?u='.$url.'" style="display: flex; align-items: center; text-decoration: none; color:#000000 !important;">
        <svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" style="margin-right: 10px;" fill="#cdab8f"><path d="M9 8h-3v4h3v12h5v-12h3.642l.358-4h-4v-1.667c0-.955.192-1.333 1.115-1.333h2.885v-5h-3.808c-3.596 0-5.192 1.583-5.192 4.615v3.385z"></path></svg> <!-- directly use the SVG string -->
        <span style="margin-left: 10px;color:#000000 !important;">Facebook</span>
        </a>
        </li>
            <li style="width: 48%; font-size: 16px; margin-bottom: 10px;">
            <a id="reddit-share-link" href="https://www.reddit.com/submit?url='.$url.'" style="display: flex; align-items: center; text-decoration: none; color:#000000 !important;">
        <svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" style="margin-right: 10px;" fill="#cdab8f"><path d="M24 11.779c0-1.459-1.192-2.645-2.657-2.645-.715 0-1.363.286-1.84.746-1.81-1.191-4.259-1.949-6.971-2.046l1.483-4.669 4.016.941-.006.058c0 1.193.975 2.163 2.174 2.163 1.198 0 2.172-.97 2.172-2.163s-.975-2.164-2.172-2.164c-.92 0-1.704.574-2.021 1.379l-4.329-1.015c-.189-.046-.381.063-.44.249l-1.654 5.207c-2.838.034-5.409.798-7.3 2.025-.474-.438-1.103-.712-1.799-.712-1.465 0-2.656 1.187-2.656 2.646 0 .97.533 1.811 1.317 2.271-.052.282-.086.567-.086.857 0 3.911 4.808 7.093 10.719 7.093s10.72-3.182 10.72-7.093c0-.274-.029-.544-.075-.81.832-.447 1.405-1.312 1.405-2.318zm-17.224 1.816c0-.868.71-1.575 1.582-1.575.872 0 1.581.707 1.581 1.575s-.709 1.574-1.581 1.574-1.582-.706-1.582-1.574zm9.061 4.669c-.797.793-2.048 1.179-3.824 1.179l-.013-.003-.013.003c-1.777 0-3.028-.386-3.824-1.179-.145-.144-.145-.379 0-.523.145-.145.381-.145.526 0 .65.647 1.729.961 3.298.961l.013.003.013-.003c1.569 0 2.648-.315 3.298-.962.145-.145.381-.144.526 0 .145.145.145.379 0 .524zm-.189-3.095c-.872 0-1.581-.706-1.581-1.574 0-.868.709-1.575 1.581-1.575s1.581.707 1.581 1.575-.709 1.574-1.581 1.574z"></path></svg> <!-- directly use the SVG string -->
        <span style="margin-left: 10px;color:#000000 !important;">Reddit</span>
        </a>
        </li>
            <li style="width: 48%; font-size: 16px; margin-bottom: 10px;">
            <a id="wa-share-link" href="https://wa.me/?text='.$url.'" style="display: flex; align-items: center; text-decoration: none; color:#000000 !important;">
        <svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" style="margin-right: 10px;" fill="#cdab8f"><path d="M.057 24l1.687-6.163c-1.041-1.804-1.588-3.849-1.587-5.946.003-6.556 5.338-11.891 11.893-11.891 3.181.001 6.167 1.24 8.413 3.488 2.245 2.248 3.481 5.236 3.48 8.414-.003 6.557-5.338 11.892-11.893 11.892-1.99-.001-3.951-.5-5.688-1.448l-6.305 1.654zm6.597-3.807c1.676.995 3.276 1.591 5.392 1.592 5.448 0 9.886-4.434 9.889-9.885.002-5.462-4.415-9.89-9.881-9.892-5.452 0-9.887 4.434-9.889 9.884-.001 2.225.651 3.891 1.746 5.634l-.999 3.648 3.742-.981zm11.387-5.464c-.074-.124-.272-.198-.57-.347-.297-.149-1.758-.868-2.031-.967-.272-.099-.47-.149-.669.149-.198.297-.768.967-.941 1.165-.173.198-.347.223-.644.074-.297-.149-1.255-.462-2.39-1.475-.883-.788-1.48-1.761-1.653-2.059-.173-.297-.018-.458.13-.606.134-.133.297-.347.446-.521.151-.172.2-.296.3-.495.099-.198.05-.372-.025-.521-.075-.148-.669-1.611-.916-2.206-.242-.579-.487-.501-.669-.51l-.57-.01c-.198 0-.52.074-.792.372s-1.04 1.016-1.04 2.479 1.065 2.876 1.213 3.074c.149.198 2.095 3.2 5.076 4.487.709.306 1.263.489 1.694.626.712.226 1.36.194 1.872.118.571-.085 1.758-.719 2.006-1.413.248-.695.248-1.29.173-1.414z"></path></svg> <!-- directly use the SVG string -->
        <span style="margin-left: 10px;color:#000000 !important;">WhatsApp</span>
        </a>
        </li>
            <li style="width: 48%; font-size: 16px; margin-bottom: 10px;">
            <a id="dc-share-link" href="discord://send?message='.$url.'" style="display: flex; align-items: center; text-decoration: none; color:#000000 !important;">
        <svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" style="margin-right: 10px;" fill="#cdab8f"><path d="M20.317 4.3698a19.7913 19.7913 0 00-4.8851-1.5152.0741.0741 0 00-.0785.0371c-.211.3753-.4447.8648-.6083 1.2495-1.8447-.2762-3.68-.2762-5.4868 0-.1636-.3933-.4058-.8742-.6177-1.2495a.077.077 0 00-.0785-.037 19.7363 19.7363 0 00-4.8852 1.515.0699.0699 0 00-.0321.0277C.5334 9.0458-.319 13.5799.0992 18.0578a.0824.0824 0 00.0312.0561c2.0528 1.5076 4.0413 2.4228 5.9929 3.0294a.0777.0777 0 00.0842-.0276c.4616-.6304.8731-1.2952 1.226-1.9942a.076.076 0 00-.0416-.1057c-.6528-.2476-1.2743-.5495-1.8722-.8923a.077.077 0 01-.0076-.1277c.1258-.0943.2517-.1923.3718-.2914a.0743.0743 0 01.0776-.0105c3.9278 1.7933 8.18 1.7933 12.0614 0a.0739.0739 0 01.0785.0095c.1202.099.246.1981.3728.2924a.077.077 0 01-.0066.1276 12.2986 12.2986 0 01-1.873.8914.0766.0766 0 00-.0407.1067c.3604.698.7719 1.3628 1.225 1.9932a.076.076 0 00.0842.0286c1.961-.6067 3.9495-1.5219 6.0023-3.0294a.077.077 0 00.0313-.0552c.5004-5.177-.8382-9.6739-3.5485-13.6604a.061.061 0 00-.0312-.0286zM8.02 15.3312c-1.1825 0-2.1569-1.0857-2.1569-2.419 0-1.3332.9555-2.4189 2.157-2.4189 1.2108 0 2.1757 1.0952 2.1568 2.419 0 1.3332-.9555 2.4189-2.1569 2.4189zm7.9748 0c-1.1825 0-2.1569-1.0857-2.1569-2.419 0-1.3332.9554-2.4189 2.1569-2.4189 1.2108 0 2.1757 1.0952 2.1568 2.419 0 1.3332-.946 2.4189-2.1568 2.4189Z"></path></svg> <!-- directly use the SVG string -->
        <span style="margin-left: 10px;color:#000000 !important;">Discord</span>
        </a>
	</li></ul></div></div>';
	return $out;
}


