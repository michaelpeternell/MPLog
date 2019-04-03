//
//  ViewController.swift
//  MPLogDemo
//
//  Created by Michael Peternell on 03.04.2019.
//  Copyright © 2019 Michael Peternell. All rights reserved.
//

import UIKit

class ViewController: UIViewController {

    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
    }

    @IBAction func onButtonPress(_ sender: UIButton?) {
        MPLogInfo("Button pressed")
        performSegue(withIdentifier: "features_segue", sender: self)
    }
    
    override func viewWillAppear(_ animated: Bool) {
        MPLogInfoStart()
        
        super.viewWillAppear(animated)
    }
}

