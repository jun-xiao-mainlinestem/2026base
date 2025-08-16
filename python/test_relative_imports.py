#!/usr/bin/env python3
"""
Test script for relative imports
"""

import sys
import os

def test_relative_imports():
    """Test that relative imports work correctly"""
    print("Testing relative imports...")
    
    try:
        # Get the current directory
        current_dir = os.path.dirname(os.path.abspath(__file__))
        src_dir = os.path.join(current_dir, 'src')
        
        print(f"Current directory: {current_dir}")
        print(f"Source directory: {src_dir}")
        
        # Add src to Python path
        if src_dir not in sys.path:
            sys.path.insert(0, src_dir)
            print(f"✓ Added {src_dir} to Python path")
        
        # Change to src directory to test relative imports
        os.chdir(src_dir)
        print(f"✓ Changed to src directory: {os.getcwd()}")
        
        # Test imports
        try:
            from rgb_template.PID import PID
            print("✓ PID import successful")
        except ImportError as e:
            print(f"✗ PID import failed: {e}")
        
        try:
            from rgb_template.drive import Drive
            print("✓ Drive import successful")
        except ImportError as e:
            print(f"✗ Drive import failed: {e}")
        
        try:
            from robot_config import chassis
            print("✓ Robot config import successful")
        except ImportError as e:
            print(f"✗ Robot config import failed: {e}")
        
        try:
            from autons import autonomous
            print("✓ Autons import successful")
        except ImportError as e:
            print(f"✗ Autons import failed: {e}")
        
    except Exception as e:
        print(f"✗ Test failed with error: {e}")
        import traceback
        traceback.print_exc()

if __name__ == "__main__":
    test_relative_imports()
    print("\nTest completed!")
