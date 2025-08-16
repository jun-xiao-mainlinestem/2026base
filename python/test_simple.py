#!/usr/bin/env python3
"""
Simple test script for VEX Python code
"""

def test_basic_syntax():
    """Test that basic Python syntax works"""
    print("Testing basic Python syntax...")
    
    # Test basic operations
    x = 5 + 3
    y = x * 2
    print(f"Basic math: {x} + 3 = {y}")
    
    # Test string operations
    message = "Hello, VEX Robot!"
    print(f"String: {message}")
    
    # Test list operations
    numbers = [1, 2, 3, 4, 5]
    total = sum(numbers)
    print(f"List sum: {total}")
    
    print("✓ Basic syntax test passed!")

def test_import_structure():
    """Test the import structure"""
    print("\nTesting import structure...")
    
    try:
        # Test importing from src directory
        import sys
        import os
        
        # Add src to Python path
        current_dir = os.path.dirname(os.path.abspath(__file__))
        src_dir = os.path.join(current_dir, 'src')
        
        if src_dir not in sys.path:
            sys.path.insert(0, src_dir)
        
        print(f"✓ Added {src_dir} to Python path")
        
        # Try to import our modules
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
        
    except Exception as e:
        print(f"✗ Import structure test failed: {e}")

if __name__ == "__main__":
    test_basic_syntax()
    test_import_structure()
    print("\nTest completed!")
