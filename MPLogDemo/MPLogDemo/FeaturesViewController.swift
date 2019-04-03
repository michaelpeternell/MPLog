//
//  FeaturesViewController.swift
//  MPLogDemo
//
//  Created by Michael Peternell on 03.04.2019.
//  Copyright © 2019 Michael Peternell. All rights reserved.
//

import UIKit

class FeaturesViewController: UIViewController {

    override func viewWillAppear(_ animated: Bool) {
        MPLogInfoStart()
        
        super.viewWillAppear(animated)
    }
    
    @IBAction func onButtonPress(_ sender: UIButton?) {
        MPLogInfo("Button pressed")
        presentingViewController?.dismiss(animated: true, completion: nil)
    }

    /*
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        // Get the new view controller using segue.destination.
        // Pass the selected object to the new view controller.
    }
    */

}
